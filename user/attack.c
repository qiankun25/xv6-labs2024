#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"


int is_valid_password(char* addr) {
    // 定义有效字符集
    char valid_chars[] = "./abcdef";
    int valid_chars_count = 8;
    
    // 检查前7个字节是否都在有效字符集中
    for (int i = 0; i < 7; i++) {
        int found = 0;
        for (int j = 0; j < valid_chars_count; j++) {
            if (addr[i] == valid_chars[j]) {
                found = 1;
                break;
            }
        }
        // 如果发现无效字符，返回0
        if (!found) {
            return 0;
        }
    }
    
    // 检查第8个字节是否为null终止符
    if (addr[7] != '\0') {
        return 0;
    }
    
    // 所有检查都通过，这是一个有效密码
    printf("find pw");
    return 1;
}


// 在内存中搜索密码的辅助函数
char* find_password_in_memory(char* start_addr, int search_size) {
    char* current = start_addr;
    char* end = start_addr + search_size - 8; // 确保有足够空间检查8字节
    
    while (current <= end) {
        if (is_valid_password(current)) {
            printf("find pw");
            return current; // 找到有效密码，返回地址
        }
        current++; // 移动到下一个字节继续搜索
    }
    
    return 0; // 未找到密码
}



int
main(int argc, char *argv[])
{
  // your code here.  you should write the secret to fd 2 using write
  // (e.g., write(2, secret, 8)

  char *end = sbrk(PGSIZE*17);
  end = end + 16 * PGSIZE;

  write(2, end + 32, 8);  // 输出找到的密码

  exit(1);
}
