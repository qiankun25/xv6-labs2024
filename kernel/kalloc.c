// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

struct {
  struct spinlock lock;
  char *superpages[NSUPERPAGES];
  int free[NSUPERPAGES];
  int initialized;
} superkmem;

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void*)PHYSTOP);
}

void
superkinit(void)
{
  initlock(&superkmem.lock, "superkmem");
  
  // 找到可用的物理内存区域并分配超页
  extern char end[]; // 内核结束位置
  char *p = (char*)PGROUNDUP((uint64)end);
  
  // 确保有足够空间，跳过一些常规页面给kalloc使用
  p += 64 * PGSIZE; // 为常规分配预留64页
  
  for(int i = 0; i < NSUPERPAGES; i++) {
    // 对齐到2MB边界
    while((uint64)p % SUPERPAGE_SIZE != 0) {
      p += PGSIZE;
    }
    
    // 确保不超出PHYSTOP
    if((uint64)p + SUPERPAGE_SIZE > PHYSTOP) {
      break;
    }
    
    superkmem.superpages[i] = p;
    superkmem.free[i] = 1;
    p += SUPERPAGE_SIZE;
  }
  
  superkmem.initialized = 1;
}

void*
superalloc(void)
{
  if(!superkmem.initialized) {
    return 0;
  }
  
  acquire(&superkmem.lock);
  for(int i = 0; i < NSUPERPAGES; i++) {
    if(superkmem.free[i] && superkmem.superpages[i]) {
      superkmem.free[i] = 0;
      void* pa = superkmem.superpages[i];
      release(&superkmem.lock);
      
      // 清零超页内容
      memset(pa, 0, SUPERPAGE_SIZE);
      return pa;
    }
  }
  release(&superkmem.lock);
  return 0;
}


void
superfree(void *pa)
{
  if(!superkmem.initialized || pa == 0) {
    return;
  }
  
  acquire(&superkmem.lock);
  for(int i = 0; i < NSUPERPAGES; i++) {
    if(superkmem.superpages[i] == pa) {
      superkmem.free[i] = 1;
      break;
    }
  }
  release(&superkmem.lock);
}

// 检查地址是否是我们管理的超页
int
issuperpage(void *pa)
{
  if(!superkmem.initialized) {
    return 0;
  }
  
  for(int i = 0; i < NSUPERPAGES; i++) {
    if(superkmem.superpages[i] == pa) {
      return 1;
    }
  }
  return 0;
}


void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
