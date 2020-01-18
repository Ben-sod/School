##clearly not finished but I gave it what I got


.text
.global  set_temp_from_ports
.data

set_temp_from_ports:
        ## assembly instructions here
        movl (%rdi), %edx                    # storing short tenths_degrees in edx
        movl 2(%rdi), %eax                   # storing char is_farenheit in eax

        movl  THERMO_SENSOR_PORT(%rip), %esi  # load global variable into register
        cmpl  $64000, %esi                   # THERMO_SENSOR_PORT>64000
        jle    .RETURNONE                      # if->return 1

        movl %esi, %r8d                       # initializing ctemp variable
        movl %esi, %r9d                       # initializing crem variable

        sarl $4, %r8d                         # THERMO_SENSOR_PORT/64 stored in ctemp
        subl $500, %r8d                        # ctemp-=500

        and $63, %r9d                         # THERMO_SENSOR_PORT%64

        cmpl $32, %r9d                        # comparing crem and 32
        jg .NEXT
        addl $1,%r8d                          # increment ctemp
        jmp .NEXT
.NEXT:
        movl  THERMO_STATUS_PORT(%rip), %esp  # load global variable into register esp
        and $1,%esp                           # perform logical and operation on THERMO_STATUS_PORT with 1
        cmpl $1,%esp                          # if(!(THERMO_STATUS_PORT & 1))
        jne .FARENHEIT
        movl $0,2(%rdi)                       # temp->is_fahrenheit = 0
        movl %r8d,(%rdi)                      # temp->tenths_degrees = ftemp
        jmp .RETURNZERO
.FARENHEIT:
        movl $1,2(%rdi)                        # temp->is_fahrenheit = 1
        imull $9,%r8d                          # ctemp*9
        movl %r8d,%eax                        # set eax to r8d
        cqto                                  # extending sign of eax to edx
        movl $5, %r10d                        # setting r10d to 5
        idivl %r10d                           # divide combined register by 5->Quotient stored in eax, Remainder in edx
        addl $320,%eax                        # adding 320, now eax holds short ftemp
        movl %eax,(%rdi)                      # temp->tenths_degrees = ftemp
        jmp .RETURNZERO

.RETURNZERO:
        movl $0,eax
        ret
.RETURNONE:
        movl $1,%eax
        ret

### Data area associated with the next function
.data


myint: .int 5                # declare location an single int
masks:                       # declaring array of masks for output
        .int 63
        .int 6
        .int 91
        .int 79
        .int 102
        .int 109
        .int 125
        .int 7
        .int 127
        .int 111


.text
.global  set_display_from_temp

## ENTRY POINT FOR REQUIRED FUNCTION
set_display_from_temp:
        ## assembly instructions here

	## two useful techniques for this problem
        movl    my_int(%rip),%eax    # load my_int into register eax
        leaq    my_array(%rip),%edx  # load pointer to beginning of my_array into edx
        movl (%rdi), %esi                    # storing short tenths_degrees in esi
        movl 2(%rdi), %ecx                   # storing char is_farenheit in ecx

        cpml $0,%ecx                         # check if is_farenheit is true
        je .ISNOTFARENHEIT
        cmpl $1,%ecx                        # temp.is_fahrenheit!=0 && temp.is_fahrenheit!=1
        cmpl %esi, $-580                    # check if tenths_degrees is less than MIN_FAHRENHEIT
        jl .RETURN1
        cpml %esi,$1220                            # check if tenths_degrees is greater than MAX_FAHRENHEIT
        jg .RETURN1
        jmp .CONTINUE
.CONTINUE:



.ISNOTFARENHEIT:
        cmpl %esi, $-500                      # check if tenths_degrees is less than MIN_CELCIUS
        jl .RETURN1
        cpml %esi,$500                            # check if tenths_degrees is greater than MAX_CELCIUS
        jg .RETURN1
        jmp .CONTINUE                          # if no conditions true, return to main part of function
.RETURN1:
        movl $1,%eax                          # set eax to 1 and return
        ret
.text
.global thermo_update

## ENTRY POINT FOR REQUIRED FUNCTION
thermo_update:
	## assembly instructions here
