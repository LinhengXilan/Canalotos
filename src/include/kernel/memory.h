/**
 * @file: include/kernel/memory.h
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
 **/

#ifndef _KERNEL_MEMORY_H
#define _KERNEL_MEMORY_H

#include <kernel/types.h>

struct Memory32
{
	u32 address1;
	u32 address2;
	u32 length1;
	u32 length2;
	u32 type;
};

struct Memory
{
	u64 address;
	u64 length;
	u32 type;
}__attribute__((packed));

struct MemoryDescriptor
{
	struct Memory memory_table[32];
	u64 memory_table_size;

	u64* bits_map;
	u64 bits_map_size;		///< number of bits in bits_map
	u64 bits_map_length;	///< length of bits_map in byte

	struct Page* pages;		///< pointer to the global pages array
	u64 pages_size;			///< number of pages array
	u64 pages_length;		///< length of pages array

	struct Zone* zones;		///< pointer to the global zones array
	u64 zones_size;			///< number of zones
	u64 zones_length;		///< length of zones

	u64 kernel_code_start;  ///< kernel should be protected
	u64 kernel_code_end;    ///< kernel should be protected
	u64 kernel_data_end;	///< kernel should be protected
	u64 kernel_end;			///< kernel should be protected

	u64 end_of_mem_desc;	///< end address of memory struct
};

#define PAGE_OFFSET ((u64)0xFFFF800000000000)

#define PAGE_4K_SHIFT 12
#define PAGE_2M_SHIFT 21
#define PAGE_1G_SHIFT 30

#define PAGE_4K (1 << PAGE_4K_SHIFT)
#define PAGE_2M (1 << PAGE_2M_SHIFT)
#define PAGE_1G (1 << PAGE_1G_SHIFT)

#define PAGE_4K_MASK (~(PAGE_4K - 1))
#define PAGE_2M_MASK (~(PAGE_2M - 1))
#define PAGE_1G_MASK (~(PAGE_1G - 1))

#define PAGE_4K_ALIGN(addr) (((u64)(addr) + PAGE_4K - 1) & PAGE_4K_MASK)
#define PAGE_2M_ALIGN(addr) (((u64)(addr) + PAGE_2M - 1) & PAGE_2M_MASK)
#define PAGE_1G_ALIGN(addr) (((u64)(addr) + PAGE_1G - 1) & PAGE_1G_MASK)

#define phy2vir(addr) ((u64*)((u64)(addr) + PAGE_OFFSET))
#define vir2phy(addr) ((u64)(addr) - PAGE_OFFSET)

/**
 * @struct Page
 * @brief memory page
 */
struct Page
{
	struct Zone* zone;	///< pointer to the zone that the page belongs to
	u64 address;		///< physical address
	u64 attribute;		///<
	u64 count;			///< reference_count of the page
	u64 age;	///<
};

/**
 * @struct Zone
 * @brief a zone of memory contains pages
 */
struct Zone
{
	struct Page* pages;						///< array of owned pages
	u64 page_size;							///< number of pages
	u64 start_address;						///< start_address of the zone
	u64 end_address;						///< end_address of the zone
	u64 capacity;							///< length of the zone
	u64 attribute;							///<
	struct MemoryDescriptor* memory_desc;   ///<
	u64 free_pages;							///< number of free pages
	u64 using_pages;						///< number of using pages
	u64 page_count;							///< reference count of the zone
};

/* page table attribute */
#define	PAGE_TABLE_PRESENT		0x0001				// bit 0  Present:			1 present;				0 no present
#define	PAGE_TABLE_RW			0x0002				// bit 1  Read Write:		1 read and write;		0 read
#define	PAGE_TABLE_US			0x0004				// bit 2  User Supervisor:	1 user and supervisor;	0 supervisor
#define PAGE_TABLE_PWT			0x0008				// bit 3  Page Level Write Through
#define PAGE_TABLE_PCD			0x0010				// bit 4  Page Level Cache Disable
#define	PAGE_TABLE_ACCESSED		0x0020				// bit 5  Accessed:			1 visited;	0 unvisited
#define	PAGE_TABLE_DIRTY		0x0040				// bit 6  Dirty:			1 dirty;	0 clean
#define	PAGE_TABLE_PS			0x0080				// bit 7  Page Size:		1 big page; 0 small page
#define	PAGE_TABLE_GLOBAL		0x0100				// bit 8  Global Page:		1 global;	0 part
#define	PAGE_TABLE_PAT			0x1000				// bit 12 Page Attribute Table
#define PAGE_TABLE_XD			0x1000000000000000	// bit 63 Execution Disable
#define PAGE_TABLE_KERNEL_GDT	(PAGE_TABLE_RW | PAGE_TABLE_PRESENT)
#define PAGE_TABLE_KERNEL_DIR	(PAGE_TABLE_RW | PAGE_TABLE_PRESENT)
#define	PAGE_TABLE_KERNEL_PAGE	(PAGE_TABLE_PS | PAGE_TABLE_RW | PAGE_TABLE_PRESENT)
#define PAGE_TABLE_USER_DIR		(PAGE_TABLE_US | PAGE_TABLE_RW | PAGE_TABLE_PRESENT)
#define	PAGE_TABLE_USER_PAGE	(PAGE_TABLE_PS | PAGE_TABLE_US | PAGE_TABLE_RW | PAGE_TABLE_PRESENT)

/* page attribute */
#define PAGE_TABLE_MAPPED	0x000
#define PAGE_KERNEL_INIT	0x001
#define PAGE_REFERENCED		0x002
#define PAGE_DIRTY			0x004
#define PAGE_ACTIVE			0x008
#define PAGE_UP_TO_DATE		0x010
#define PAGE_DEVICE			0x020
#define PAGE_KERNEL			0x040
#define PAGE_KERNEL_SHARED	0x080
#define PAGE_SLAB			0x100

/* zone attribute */
#define ZONE_DMA		0x1
#define ZONE_NORMAL		0x2
#define ZONE_UNMAPPED	0x4

#define flush_tlb()	\
do	\
{	\
	u64 temp;	\
	__asm__ __volatile__(	\
		"	movq	%%cr3, %0	\n\t"	\
		"	movq	%0, %%cr3	\n\t"	\
		: "=r"(temp)	\
		:	\
		: "memory"	\
	);	\
} while(0);

inline u64* get_gdt()
{
	u64* temp;
	__asm__ __volatile__(
		"	movq	%%cr3, %0	\n\t"
		: "=r"(temp)
		:
		: "memory"
	);
	return temp;
}

struct Page* alloc_page(u64 zone_type, u64 number, u64 flags);

#endif