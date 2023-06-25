.intel_syntax noprefix

.section .text

.extern exception_handler
.global interrupt_intermediary

.altmacro

.macro intstub_no_err i
  interrupt_stub_\i:
    push 0
    push \i
    jmp interrupt_intermediary
.endm

.macro intstub_err i
  interrupt_stub_\i:
    push \i
    jmp interrupt_intermediary
.endm

.macro intstub_label i
  .quad interrupt_stub_\i
.endm

interrupt_intermediary:
  push rax
  push rbx
  push rcx
  push rdx
  push rsp
  push rbp
  push rsi
  push rdi
  push r8
  push r9
  push r10
  push r11
  push r12
  push r13
  push r14
  push r15

  mov rax, cr0
  push rax
  mov rax, cr2
  push rax
  mov rax, cr3
  push rax
  mov rax, cr4
  push rax

  xor rax, rax
  mov ax, ds

  mov ax, 0x30
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov rdi, rsp // pointer to interrupt info
  
  call exception_handler

  add rsp, 32 // control registers

  pop r15
  pop r14
  pop r13
  pop r12
  pop r11
  pop r10
  pop r9
  pop r8
  pop rdi
  pop rsi
  pop rbp
  pop rsp
  pop rdx
  pop rcx
  pop rbx
  pop rax
  
  add rsp, 16
  iretq

intstub_no_err 0
intstub_no_err 1
intstub_no_err 2
intstub_no_err 3
intstub_no_err 4
intstub_no_err 5
intstub_no_err 6
intstub_no_err 7
intstub_err 8
intstub_no_err 9
intstub_err 10
intstub_err 11
intstub_err 12
intstub_err 13
intstub_err 14
intstub_no_err 15
intstub_no_err 16
intstub_err 17
intstub_no_err 18
intstub_no_err 19
intstub_no_err 20
intstub_err 21
intstub_no_err 22
intstub_no_err 23
intstub_no_err 24
intstub_no_err 25
intstub_no_err 26
intstub_no_err 27
intstub_no_err 28
intstub_err 29
intstub_err 30
intstub_no_err 31
intstub_no_err 32
intstub_no_err 33
intstub_no_err 34
intstub_no_err 35
intstub_no_err 36
intstub_no_err 37
intstub_no_err 38
intstub_no_err 39
intstub_no_err 40
intstub_no_err 41
intstub_no_err 42
intstub_no_err 43
intstub_no_err 44
intstub_no_err 45
intstub_no_err 46
intstub_no_err 47
intstub_no_err 48
intstub_no_err 49
intstub_no_err 50
intstub_no_err 51
intstub_no_err 52
intstub_no_err 53
intstub_no_err 54
intstub_no_err 55
intstub_no_err 56
intstub_no_err 57
intstub_no_err 58
intstub_no_err 59
intstub_no_err 60
intstub_no_err 61
intstub_no_err 62
intstub_no_err 63
intstub_no_err 64
intstub_no_err 65
intstub_no_err 66
intstub_no_err 67
intstub_no_err 68
intstub_no_err 69
intstub_no_err 70
intstub_no_err 71
intstub_no_err 72
intstub_no_err 73
intstub_no_err 74
intstub_no_err 75
intstub_no_err 76
intstub_no_err 77
intstub_no_err 78
intstub_no_err 79
intstub_no_err 80
intstub_no_err 81
intstub_no_err 82
intstub_no_err 83
intstub_no_err 84
intstub_no_err 85
intstub_no_err 86
intstub_no_err 87
intstub_no_err 88
intstub_no_err 89
intstub_no_err 90
intstub_no_err 91
intstub_no_err 92
intstub_no_err 93
intstub_no_err 94
intstub_no_err 95
intstub_no_err 96
intstub_no_err 97
intstub_no_err 98
intstub_no_err 99
intstub_no_err 100
intstub_no_err 101
intstub_no_err 102
intstub_no_err 103
intstub_no_err 104
intstub_no_err 105
intstub_no_err 106
intstub_no_err 107
intstub_no_err 108
intstub_no_err 109
intstub_no_err 110
intstub_no_err 111
intstub_no_err 112
intstub_no_err 113
intstub_no_err 114
intstub_no_err 115
intstub_no_err 116
intstub_no_err 117
intstub_no_err 118
intstub_no_err 119
intstub_no_err 120
intstub_no_err 121
intstub_no_err 122
intstub_no_err 123
intstub_no_err 124
intstub_no_err 125
intstub_no_err 126
intstub_no_err 127
intstub_no_err 128
intstub_no_err 129
intstub_no_err 130
intstub_no_err 131
intstub_no_err 132
intstub_no_err 133
intstub_no_err 134
intstub_no_err 135
intstub_no_err 136
intstub_no_err 137
intstub_no_err 138
intstub_no_err 139
intstub_no_err 140
intstub_no_err 141
intstub_no_err 142
intstub_no_err 143
intstub_no_err 144
intstub_no_err 145
intstub_no_err 146
intstub_no_err 147
intstub_no_err 148
intstub_no_err 149
intstub_no_err 150
intstub_no_err 151
intstub_no_err 152
intstub_no_err 153
intstub_no_err 154
intstub_no_err 155
intstub_no_err 156
intstub_no_err 157
intstub_no_err 158
intstub_no_err 159
intstub_no_err 160
intstub_no_err 161
intstub_no_err 162
intstub_no_err 163
intstub_no_err 164
intstub_no_err 165
intstub_no_err 166
intstub_no_err 167
intstub_no_err 168
intstub_no_err 169
intstub_no_err 170
intstub_no_err 171
intstub_no_err 172
intstub_no_err 173
intstub_no_err 174
intstub_no_err 175
intstub_no_err 176
intstub_no_err 177
intstub_no_err 178
intstub_no_err 179
intstub_no_err 180
intstub_no_err 181
intstub_no_err 182
intstub_no_err 183
intstub_no_err 184
intstub_no_err 185
intstub_no_err 186
intstub_no_err 187
intstub_no_err 188
intstub_no_err 189
intstub_no_err 190
intstub_no_err 191
intstub_no_err 192
intstub_no_err 193
intstub_no_err 194
intstub_no_err 195
intstub_no_err 196
intstub_no_err 197
intstub_no_err 198
intstub_no_err 199
intstub_no_err 200
intstub_no_err 201
intstub_no_err 202
intstub_no_err 203
intstub_no_err 204
intstub_no_err 205
intstub_no_err 206
intstub_no_err 207
intstub_no_err 208
intstub_no_err 209
intstub_no_err 210
intstub_no_err 211
intstub_no_err 212
intstub_no_err 213
intstub_no_err 214
intstub_no_err 215
intstub_no_err 216
intstub_no_err 217
intstub_no_err 218
intstub_no_err 219
intstub_no_err 220
intstub_no_err 221
intstub_no_err 222
intstub_no_err 223
intstub_no_err 224
intstub_no_err 225
intstub_no_err 226
intstub_no_err 227
intstub_no_err 228
intstub_no_err 229
intstub_no_err 230
intstub_no_err 231
intstub_no_err 232
intstub_no_err 233
intstub_no_err 234
intstub_no_err 235
intstub_no_err 236
intstub_no_err 237
intstub_no_err 238
intstub_no_err 239
intstub_no_err 240
intstub_no_err 241
intstub_no_err 242
intstub_no_err 243
intstub_no_err 244
intstub_no_err 245
intstub_no_err 246
intstub_no_err 247
intstub_no_err 248
intstub_no_err 249
intstub_no_err 250
intstub_no_err 251
intstub_no_err 252
intstub_no_err 253
intstub_no_err 254
intstub_no_err 255

.section .data

.global interrupt_stub_table
interrupt_stub_table:
  .set i, 0
  .rept 256
    intstub_label %i
    .set i, i+1
  .endr
