/**
 * @file: kernel/memory.c
 * @author: LinhengXilan
 * @data: 2025-9-26
 * @version: build15
 **/

#include <kernel/memory.h>
#include <kernel/printk.h>
#include <kernel/const.h>
#include <kernel/global.h>
#include <kernel/lib/string.h>

u64 page_init(struct Page* page, u64 flags);
u64 page_clean(struct Page* page);
struct Page* alloc_page(u64 zone_type, u64 number, u64 flags);

/**
 * @note Default use 2M page.
 */
void init_memory()
{
	 color_printk(ORANGE, BLACK, "init_memory()\n");
	 color_printk(GREEN, BLACK, "memory type: 1.RAM\t2.ROM/Reserved\t3.ACPI Reclaim Memory\t4.ACPI NVS Memory/Undefined\n");
	// Set kernel length.
	memory_desc.kernel_code_start = (u64)&_text;
	memory_desc.kernel_code_end = (u64)&_etext;
	memory_desc.kernel_data_end = (u64)&_edata;
	memory_desc.kernel_end = (u64)&_end;
	// Get memory struct.
	struct Memory* memory = (struct Memory*)0xFFFF800000007E00;
	u64 total_memory = 0;
	for (int i = 0; i < 32; i++)
	{
		if (memory->type == 1)
		{
			total_memory += memory->length;
		}
		color_printk(GREEN, BLACK, "Address:%16lx\tLength:%16lx\tType:%x\n", memory->address, memory->length, memory->type);
		memory_desc.memory_table[i].address = memory->address;
		memory_desc.memory_table[i].length = memory->length;
		memory_desc.memory_table[i].type = memory->type;
		memory_desc.memory_table_size = i;
		memory++;
		if (memory->type > 4 || memory->length == 0 || memory->type < 1)
		{
			break;
		}
	}
	color_printk(GREEN, BLACK, "Total memory size:%16lx\n", total_memory);
	total_memory = 0;
	for(int i = 0; i <= memory_desc.memory_table_size; i++)
	{
		u64 start, end;
		if (memory_desc.memory_table[i].type != 1)
		{
			continue;
		}
		start = PAGE_2M_ALIGN(memory_desc.memory_table[i].address);
		end = ((memory_desc.memory_table[i].address + memory_desc.memory_table[i].length) >> PAGE_2M_SHIFT) << PAGE_2M_SHIFT;
		if (end <= start)
		{
			continue;
		}
		total_memory += (end - start) >> PAGE_2M_SHIFT;
	}
	// Get total memory.
	color_printk(GREEN, BLACK, "Number of 2M pages:%8x\n", total_memory);
	total_memory = memory_desc.memory_table[memory_desc.memory_table_size].address + memory_desc.memory_table[memory_desc.memory_table_size].length;
	color_printk(GREEN, BLACK, "Total memory size:%16lx\n", total_memory);
	// init bits map.
	memory_desc.bits_map = (u64*)(memory_desc.kernel_end + PAGE_4K - 1 & PAGE_4K_MASK);
	memory_desc.bits_map_size = total_memory >> PAGE_2M_SHIFT;
	memory_desc.bits_map_length = ((total_memory >> PAGE_2M_SHIFT) + sizeof(u64) * 8 - 1) / 8 & ~(sizeof(u64) - 1);
	memset(memory_desc.bits_map, 0xFF, memory_desc.bits_map_length);
	// init pages struct.
	memory_desc.pages = (struct Page*)((u64)memory_desc.bits_map + memory_desc.bits_map_length + PAGE_4K - 1 & PAGE_4K_MASK);
	memory_desc.pages_size = total_memory >> PAGE_2M_SHIFT;
	memory_desc.pages_length = (total_memory >> PAGE_2M_SHIFT) * sizeof(struct Page) + sizeof(u64) - 1 & ~(sizeof(u64) - 1);
	memset(memory_desc.pages, 0, memory_desc.pages_length);
	// init zones struct.
	memory_desc.zones = (struct Zone*)((u64)memory_desc.pages + memory_desc.pages_length + PAGE_4K - 1 & PAGE_4K_MASK);
	memory_desc.zones_size = 0;
	memory_desc.zones_length = 5 * sizeof(struct Zone) + sizeof(u64) - 1 & ~(sizeof(u64) - 1);
	memset(memory_desc.zones, 0, memory_desc.zones_length);

	for (int i = 0; i < memory_desc.memory_table_size; i++)
	{
		if (memory_desc.memory_table[i].type != 1)
		{
			continue;
		}
		const u64 start = PAGE_2M_ALIGN(memory_desc.memory_table[i].address);
		const u64 end = ((memory_desc.memory_table[i].address + memory_desc.memory_table[i].length) >> PAGE_2M_SHIFT) << PAGE_2M_SHIFT;
		if (end <= start)
		{
			continue;
		}
		// Setup zones.
		struct Zone* zone = memory_desc.zones + memory_desc.zones_size;
		memory_desc.zones_size++;
		zone->start_address = start;
		zone->end_address = end;
		zone->capacity = end - start;
		zone->using_pages = 0;
		zone->free_pages = zone->capacity >> PAGE_2M_SHIFT;
		zone->page_count = 0;
		zone->attribute = 0;
		zone->memory_desc = &memory_desc;
		zone->page_size = zone->free_pages;
		zone->pages = memory_desc.pages + (start >> PAGE_2M_SHIFT);
		// Setup each page of the zone
		struct Page* page = zone->pages;
		for (int j = 0; j < zone->page_size; j++, page++)
		{
			page->zone = zone;
			page->address = start + PAGE_2M * j;
			page->attribute = 0;
			page->count = 0;
			page->age = 0;
			*(memory_desc.bits_map + ((page->address >> PAGE_2M_SHIFT) >> 6)) ^= 1ul << (page->address >> PAGE_2M_SHIFT) % 64;
		}
	}
	// Setup page0 because type of memory0~2M is not always 1, but it contains the kernel.
	memory_desc.pages->zone = memory_desc.zones;
	memory_desc.pages->address = 0;
	memory_desc.pages->attribute = 0;
	memory_desc.pages->count = 0;
	memory_desc.pages->age = 0;
	memory_desc.zones_length = memory_desc.zones_size * sizeof(struct Zone) + sizeof(u64) - 1 & ~(sizeof(u64) - 1);

	color_printk(GREEN, BLACK, "bits_map:%16lx, bits_size:%16lx, bits_length:%16lx\n", memory_desc.bits_map, memory_desc.bits_map_size, memory_desc.bits_map_length);
	color_printk(GREEN, BLACK, "pages_struct:%16lx,pages_size:%16lx,pages_length:%16lx\n", memory_desc.pages, memory_desc.pages_size, memory_desc.pages_length);
	color_printk(GREEN, BLACK, "zones_struct:%16lx,zones_size:%16lx,zones_length:%16lx\n", memory_desc.zones, memory_desc.zones_size, memory_desc.zones_length);

	ZONE_DMA_INDEX = 0;
	ZONE_NORMAL_INDEX = 0;
	for (int i = 0; i < memory_desc.zones_size; i++)
	{
		struct Zone* zone = memory_desc.zones + i;
		color_printk(GREEN,BLACK,"zone_start_address:%16lx, zone_end_address:%16lx, zone_length:%16lx, pages_group:%16lx, pages_length:%16lx\n", zone->start_address, zone->end_address, zone->capacity, zone->pages, zone->page_size);
		if (zone->start_address == 0x100000000)
		{
			ZONE_UNMAPPED_INDEX = i;
		}
	}
	memory_desc.end_of_mem_desc = (u64)memory_desc.zones + memory_desc.zones_length + sizeof(u64) * 32 & ~(sizeof(u64) - 1);
	color_printk(GREEN, BLACK, "start_code:%16lx, end_code:%16lx, end_data:%16lx, end_brk:%16lx, end_of_struct:%16lx\n", memory_desc.kernel_code_start, memory_desc.kernel_code_end, memory_desc.kernel_data_end, memory_desc.kernel_end, memory_desc.end_of_mem_desc);
	int size = vir2phy(memory_desc.end_of_mem_desc) >> PAGE_2M_SHIFT;
	for (int i = 0; i <= size; i++)
	{
		page_init(memory_desc.pages + i, PAGE_TABLE_MAPPED | PAGE_KERNEL_INIT | PAGE_ACTIVE | PAGE_KERNEL);
	}
	Global_CR3 = get_gdt();
	color_printk(GREEN, BLACK, "Global_CR3\t:%16lx\n", Global_CR3);
	color_printk(GREEN, BLACK, "*Global_CR3\t:%16lx\n", *phy2vir(Global_CR3) & ~0xff);
	color_printk(GREEN, BLACK, "**Global_CR3\t:%16lx\n", *phy2vir(*phy2vir(Global_CR3) & ~0xff) & ~0xff);

	flush_tlb();
}

/**
 * @param page 需要初始化的页的指针
 * @param flags 页的标志
 * @return 0
 */
u64 page_init(struct Page* page, u64 flags)
{
	if (!page->attribute)
	{
		*(memory_desc.bits_map + ((page->address >> PAGE_2M_SHIFT) >> 6)) |= 1ul << (page->address >> PAGE_2M_SHIFT) % 64;
		page->attribute = flags;
		page->count++;
		page->zone->using_pages++;
		page->zone->free_pages--;
		page->zone->page_count++;
	}
	else if (page->attribute & PAGE_REFERENCED || page->attribute & PAGE_KERNEL_SHARED || flags & PAGE_REFERENCED || flags & PAGE_KERNEL_SHARED)
	{
		page->attribute |= flags;
		page->count++;
		page->zone->page_count++;
	}
	else
	{
		*(memory_desc.bits_map + ((page->address >> PAGE_2M_SHIFT) >> 6)) |= 1ul << (page->address >> PAGE_2M_SHIFT) % 64;
		page->attribute |= flags;
	}
	return 0;
}

u64 page_clean(struct Page* page)
{
	if (!page->attribute)
	{
		page->attribute = 0;
	}
	else if(page->attribute & PAGE_REFERENCED || page->attribute & PAGE_KERNEL_SHARED)
	{
		page->count--;
		page->zone->page_count--;
		if (!page->count)
		{
			page->attribute = 0;
			page->zone->using_pages--;
			page->zone->free_pages++;
		}
	}
	else
	{
		*(memory_desc.bits_map + ((page->address >> PAGE_2M_SHIFT) >> 6)) &= ~(1ul << (page->address >> PAGE_2M_SHIFT) % 64);
		page->attribute = 0;
		page->count = 0;
		page->zone->using_pages--;
		page->zone->free_pages++;
		page->zone->page_count--;
	}
	return 0;
}

/**
 * @param zone_type zone of tha page to be allocated
 * @param number number of pages
 * @param flags flags of page
 * @return pointer of the first allocated page
 * @note Number should not bigger than 64.
 */
struct Page* alloc_page(u64 zone_type, u64 number, u64 flags)
{
	int zone_start = 0;
	int zone_end = 0;
	// The switch is temporarily useless.
	switch (zone_type)
	{
	case ZONE_DMA:
		zone_start = 0;
		zone_end = ZONE_DMA_INDEX;
		break;
	case ZONE_NORMAL:
		zone_start = ZONE_DMA_INDEX;
		zone_end = ZONE_NORMAL_INDEX;
		break;
	case ZONE_UNMAPPED:
		zone_start = ZONE_UNMAPPED_INDEX;
		zone_end = memory_desc.zones_size - 1;
		break;
	default:
		color_printk(RED, BLACK, "alloc_page error!\n");
		return nullptr;
	}

	for (int i = zone_start; i <= zone_end; i++)
	{
		// Search for a zone with enough pages.
		if ((memory_desc.zones + i)->free_pages < number)
		{
			continue;
		}
		struct Zone* zone = memory_desc.zones + i;
		u64 start = zone->start_address >> PAGE_2M_SHIFT;
		u64 end = zone->end_address >> PAGE_2M_SHIFT;
		u64 temp = 64 - start % 64;
		for (u64 j = start; j <= end; j += j % 64 ? temp : 64)
		{
			u64* bit = memory_desc.bits_map + (j >> 6);
			u64 shift = j % 64;
			for (u64 k = shift; k < 64 - shift; k++)
			{
				if (!((*bit >> k | *(bit + 1) << (64 - k)) & (number == 64 ? 0xFFFFFFFFFFFFFFFFul : (1UL << number) - 1)))	// combine u64 high and next u64 low
				{
					u64 n_page = j + k - 1;
					for (u64 l = 0; l < number; l++)
					{
						struct Page* page = memory_desc.pages + n_page + l;
						page_init(page, flags);
					}
					return memory_desc.pages + n_page;
				}
			}
		}
	}
	return nullptr;
}