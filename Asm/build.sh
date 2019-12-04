as payload_final.s -o payload_final.o
ld -lC -no_pie -allow_stack_execute -macosx_version_min 10.14.0 payload_final.o -o payload_final
