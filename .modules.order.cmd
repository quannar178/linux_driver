cmd_/home/narhint/my_driver/modules.order := {   echo /home/narhint/my_driver/hello_kernel_module.ko; :; } | awk '!x[$$0]++' - > /home/narhint/my_driver/modules.order
