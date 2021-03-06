#include <stdio.h>
#include <string.h>

#include "lg/asm.h"
#include "lg/init.h"
#include "lg/stack.h"
#include "lg/timer.h"
#include "lg/vm.h"

int main(int argc, char *argv[]) {
  lg_init();
  struct lg_vm vm;
  lg_vm_init(&vm);
  vm.debug = true;
  
  struct lg_stack stack;
  lg_stack_init(&stack);

  uintmax_t reps = 1;
  
  while (--argc && ++argv) {
    const char *a = *argv;

    if (strcmp(a, "--debug") == 0) {
      vm.debug = true;
    } else if (strcmp(a, "--bench") == 0) {
      reps = strtoumax(*(++argv), NULL, 10);
      argc--;
    } else {
      if (!lg_asm(&vm, a)) {
	break;
      }	      
    }
  }

  lg_emit(&vm, LG_STOP);

  struct lg_timer t;
  lg_timer_init(&t);
  
  for (int i = 0; i < reps; i++) {
    lg_exec(&vm, &stack, 0);
  }

  if (reps > 1) {
    printf("%" PRIu64 "us\n", lg_timer_usecs(&t));
  }
  
  if (vm.debug) {
    lg_stack_deinit(&stack);
    lg_vm_deinit(&vm);
    lg_deinit();
  }
  
  return 0;
}
