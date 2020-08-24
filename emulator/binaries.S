
              .global qemu_start
              .global qemu_end
              .global uefi_start
              .global uefi_end
              .global artos_start
              .global artos_end

              .text

qemu_start:   .incbin "/usr/share/qemu-win.tar"
qemu_end:

uefi_start:   .incbin "/usr/share/qemu-efi-aarch64/QEMU_EFI.fd"
uefi_end:

artos_start:  .incbin "../system/artos-virt-1.0.efi"
artos_end:
