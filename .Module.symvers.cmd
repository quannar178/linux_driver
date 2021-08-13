cmd_/home/narhint/my_driver/Module.symvers := sed 's/ko$$/o/' /home/narhint/my_driver/modules.order | scripts/mod/modpost -m -a   -o /home/narhint/my_driver/Module.symvers -e -i Module.symvers   -T -
