
#ifdef NEED_UNDERSCORE
# define TRANSFORM_SYMBOLS
# define EXPORT_SYMBOL(sym) _ ## sym
#endif

#ifndef TRANSFORM_SYMBOLS

# define GLOBAL(sym) \
  .global sym; \
  sym ##:

# define GLOBAL_FUNC(sym) \
  GLOBAL (sym)

#else

# define GLOBAL(sym) \
  .global EXPORT_SYMBOL (sym); \
  EXPORT_SYMBOL (sym) ##: \
  sym ##:

# define GLOBAL_FUNC(sym) \
  .global sym; \
  GLOBAL (sym)

#endif

.text

/*----------------------------------------------------------------*/
GLOBAL_FUNC (MSSLOWSOUNDVOLPAN)
/*----------------------------------------------------------------*/
		mov    0x8(%esp),%eax
		cmp    $0x8000,%eax
		jbe    jump_fbcf0
		mov    $0x8000,%eax
	jump_fbcf0:
		mov    $0x8000,%ebx
		mov    $0x4f6,%ecx
		mul    %ecx
		div    %ebx
		mov    $0xa,%ebx
		add    $0x5,%eax
		xor    %edx,%edx
		div    %ebx
		push   %eax
		mov    0x10(%esp),%eax
		mov    0x38(%eax),%edx
		push   %edx
		call   AIL_set_sample_volume
		add    $0x8,%esp
		mov    $0x4f6,%ecx
		mov    $0x10000,%ebx
		mov    0x4(%esp),%eax
		mul    %ecx
		div    %ebx
		mov    $0xa,%ebx
		add    $0x5,%eax
		xor    %edx,%edx
		div    %ebx
		push   %eax
		mov    0x10(%esp),%eax
		mov    0x38(%eax),%ebx
		push   %ebx
		call   AIL_set_sample_pan
		add    $0x8,%esp
		ret    $0xc


/*----------------------------------------------------------------*/
GLOBAL_FUNC (MSSSMACKTIMERSETUP)
/*----------------------------------------------------------------*/
		mov    data_1592dc,%edx
		dec    %edx
		mov    %edx,data_1592dc
		jne    jump_fbd8f
		mov    data_1e86b0,%ecx
		push   %ecx
		call   AIL_release_timer_handle
		add    $0x4,%esp
		push   $MSSSMACKTIMERDONE
		push   $ac_MSSSMACKTIMERREAD
		call   VMM_unlock_range
		add    $0x8,%esp
		push   $0x4
		push   $data_1592d8
		call   AIL_vmm_unlock
		add    $0x8,%esp
	jump_fbd8f:
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (MSSSMACKTIMERDONE)
/*----------------------------------------------------------------*/
		push   %esi
		push   %edi
		mov    data_1592dc,%edx
		inc    %edx
		mov    %edx,data_1592dc
		cmp    $0x1,%edx
		jne    jump_fbe2e
		push   $MSSSMACKTIMERDONE
		push   $ac_MSSSMACKTIMERREAD
		call   VMM_lock_range
		add    $0x8,%esp
		push   $0x4
		push   $data_1592d8
		call   AIL_vmm_lock
		add    $0x8,%esp
		push   $func_fbeb0
		call   AIL_register_timer
		add    $0x4,%esp
		mov    %eax,data_1e86b0
		mov    _LowUnk_1e86c0,%ecx
		push   %ecx
		mov    data_1e86b0,%esi
		push   %esi
		call   AIL_set_timer_frequency
		add    $0x8,%esp
		mov    data_1e86b0,%edi
		push   %edi
		call   AIL_start_timer
		add    $0x4,%esp
		call   AIL_interrupt_divisor
		mov    %eax,%edx
		mov    %edx,%ebx
#if 0
		mov    $0x34,%al
		out    %al,$0x43
		jmp    jump_fbe0f
	jump_fbe0f:
		mov    %bl,%al
		out    %al,$0x40
		jmp    jump_fbe15
	jump_fbe15:
		mov    %bh,%al
		out    %al,$0x40
		mov    data_1592d8,%eax
		cmp    data_1592d8,%eax
		jne    jump_fbe2e
	jump_fbe26:
		cmp    data_1592d8,%eax
		je     jump_fbe26
#endif
	jump_fbe2e:
		pop    %edi
		pop    %esi
		ret


/*----------------------------------------------------------------*/
MSSSMACKTIMERREAD:
/*----------------------------------------------------------------*/
		push   %esi
		push   %edi
	jump_fbe42:
		mov    data_1592d8,%edx
		xor    %al,%al
		out    %al,$0x43
		jmp    jump_fbe4e
	jump_fbe4e:
		in     $0x40,%al
		mov    %al,%ah
		in     $0x40,%al
		xchg   %al,%ah
		and    $0xffff,%eax
		mov    %eax,%esi
		cmp    data_1592d8,%edx
		jne    jump_fbe42
		mov    $0x3e8,%ecx
		mov    _LowUnk_1e86c0,%ebx
		mov    %edx,%eax
		mul    %ecx
		div    %ebx
		mov    $0x1234dd,%ebx
		mov    %eax,%edi
		mov    _LowSoundVolPanAddr,%eax
		mov    $0x3e8,%ecx
		sub    %esi,%eax
		mul    %ecx
		div    %ebx
		mov    data_1e86b4,%ecx
		lea    (%edi,%eax,1),%edx
		cmp    %ecx,%edx
		jae    jump_fbea1
		mov    %ecx,%edx
		mov    %edx,%eax
		pop    %edi
		pop    %esi
		ret
	jump_fbea1:
		mov    %edx,data_1e86b4
		mov    %edx,%eax
		pop    %edi
		pop    %esi
		ret


/*----------------------------------------------------------------*/
func_fbeb0:
/*----------------------------------------------------------------*/
		mov    data_1592d8,%eax
		incl   data_1592d8
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (MSSLOWSOUNDCLOSE)
/*----------------------------------------------------------------*/
		push   %esi
		push   %edi
		push   %ebp
		mov    0x10(%esp),%esi
		cmpl   $0x0,_fss
		je     jump_fbf34
		mov    0x38(%esi),%ebx
		push   %ebx
		call   AIL_end_sample
		add    $0x4,%esp
		mov    0x38(%esi),%ecx
		push   %ecx
		call   AIL_release_sample_handle
		add    $0x4,%esp
		mov    0x3c(%esi),%eax
		add    %eax,%eax
		push   %eax
		mov    0x54(%esi),%edi
		push   %edi
		call   AIL_vmm_unlock
		add    $0x8,%esp
		mov    0x54(%esi),%ebp
		push   %ebp
		call   RADFREE
		mov    _fss,%eax
		cmp    %eax,%esi
		jne    jump_fbf16
		mov    0x34(%eax),%eax
		mov    %eax,_fss
		jmp    jump_fbf34
	jump_fbf16:
		cmpl   $0x0,0x34(%eax)
		je     jump_fbf34
	jump_fbf1c:
		mov    0x34(%eax),%ecx
		cmp    %ecx,%esi
		jne    jump_fbf2b
		mov    0x34(%ecx),%esi
		mov    %esi,0x34(%eax)
		jmp    jump_fbf34
	jump_fbf2b:
		mov    0x34(%eax),%eax
		cmpl   $0x0,0x34(%eax)
		jne    jump_fbf1c
	jump_fbf34:
		pop    %ebp
		pop    %edi
		pop    %esi
		ret    $0x4


/*----------------------------------------------------------------*/
GLOBAL_FUNC (MSSLOWSOUNDPURGE)
/*----------------------------------------------------------------*/
		push   %esi
		mov    0x8(%esp),%esi
		mov    0x38(%esi),%edx
		push   %edx
		call   AIL_end_sample
		add    $0x4,%esp
		movl   $0x0,0x50(%esi)
		movl   $0x0,0x68(%esi)
		mov    %esi,%eax
		movl   $0x0,0x6c(%esi)
		call   doinit_
		pop    %esi
		ret    $0x4


/*----------------------------------------------------------------*/
GLOBAL_FUNC (MSSLOWSOUNDOPEN)
/*----------------------------------------------------------------*/
		push   %esi
		push   %edi
		push   %ebp
		mov    0x14(%esp),%esi
		cmpl   $0x0,SmackMSSDigDriver
		jne    jump_fc041
		cmpl   $0x0,MSSLiteInit
		je     jump_fc026
		testb  $0x20,0x10(%esp)
		jne    jump_fbfae
		cmpl   $0x0,0x4c(%esi)
		je     jump_fbfb5
	jump_fbfae:
		mov    $0x1,%eax
		jmp    jump_fbfb7
	jump_fbfb5:
		xor    %eax,%eax
	jump_fbfb7:
		push   %eax
		push   $0x7
		call   AIL_set_preference
		add    $0x8,%esp
		cmpl   $0x0,0x48(%esi)
		setne  %al
		and    $0xff,%eax
		push   %eax
		push   $0x8
		call   AIL_set_preference
		add    $0x8,%esp
		push   $0x0
		push   $data_15f7dc
		call   AIL_install_DIG_driver_file
		add    $0x8,%esp
		mov    %eax,SmackMSSDigDriver
		mov    SmackMSSDigDriver,%ebp
		test   %ebp,%ebp
		jne    jump_fc026
		push   %ebp
		push   $data_15f7e8
		call   AIL_install_DIG_driver_file
		add    $0x8,%esp
		mov    %eax,SmackMSSDigDriver
		cmpl   $0x0,SmackMSSDigDriver
		jne    jump_fc026
		push   %ebp
		push   $data_15f7f4
		call   AIL_install_DIG_driver_file
		add    $0x8,%esp
		mov    %eax,SmackMSSDigDriver
	jump_fc026:
		cmpl   $0x0,SmackMSSDigDriver
		jne    jump_fc041
		mov    $0xffffffff,%ecx
		xor    %al,%al
		mov    %ecx,SmackMSSDigDriver
		jmp    jump_fc12c
	jump_fc041:
		cmpl   $0xffffffff,SmackMSSDigDriver
		jne    jump_fc052
		xor    %al,%al
		pop    %ebp
		pop    %edi
		pop    %esi
		ret    $0x8
	jump_fc052:
		cmpl   $0x0,0x48(%esi)
		je     jump_fc06c
		cmpl   $0x0,0x4c(%esi)
		je     jump_fc065
		mov    $0x3,%eax
		jmp    jump_fc07b
	jump_fc065:
		mov    $0x1,%eax
		jmp    jump_fc07b
	jump_fc06c:
		cmpl   $0x0,0x4c(%esi)
		je     jump_fc079
		mov    $0x2,%eax
		jmp    jump_fc07b
	jump_fc079:
		xor    %eax,%eax
	jump_fc07b:
		push   %eax
		mov    0x40(%esi),%eax
		push   %eax
		mov    SmackMSSDigDriver,%edx
		push   %edx
		call   ail_minimum_sample_buffer_size
		add    $0x3,%eax
		and    $0xfc,%al
		add    $0xc,%esp
		mov    %eax,0x64(%esi)
		mov    0x14(%esi),%eax
		shr    $0x2,%eax
		add    $0xfff,%eax
		xor    %al,%al
		and    $0xf0,%ah
		mov    %eax,0x3c(%esi)
		mov    0x64(%esi),%eax
		cmp    0x3c(%esi),%eax
		jbe    jump_fc0b5
		mov    %eax,0x3c(%esi)
	jump_fc0b5:
		mov    0x3c(%esi),%eax
		add    %eax,%eax
		push   %eax
		call   RADMALLOC
		mov    %eax,0x54(%esi)
		test   %eax,%eax
		jne    jump_fc0cf
		xor    %al,%al
		pop    %ebp
		pop    %edi
		pop    %esi
		ret    $0x8
	jump_fc0cf:
		mov    0x3c(%esi),%eax
		add    %eax,%eax
		push   %eax
		mov    0x54(%esi),%edi
		push   %edi
		call   AIL_vmm_lock
		add    $0x8,%esp
		mov    SmackMSSDigDriver,%ebp
		push   %ebp
		call   AIL_allocate_sample_handle
		add    $0x4,%esp
		mov    %eax,0x38(%esi)
		cmpl   $0x0,0x38(%esi)
		jne    jump_fc10a
		mov    0x54(%esi),%ebx
		push   %ebx
		call   RADFREE
		xor    %al,%al
		pop    %ebp
		pop    %edi
		pop    %esi
		ret    $0x8
	jump_fc10a:
		mov    0x54(%esi),%eax
		mov    0x3c(%esi),%edx
		add    %edx,%eax
		mov    %eax,0x58(%esi)
		mov    %esi,%eax
		call   doinit_
		mov    _fss,%eax
		mov    %esi,_fss
		mov    %eax,0x34(%esi)
		mov    $0x1,%al
	jump_fc12c:
		pop    %ebp
		pop    %edi
		pop    %esi
		ret    $0x8


/*----------------------------------------------------------------*/
doinit_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		mov    %eax,%esi
		mov    0x38(%eax),%edx
		push   %edx
		call   AIL_init_sample
		add    $0x4,%esp
		mov    0x48(%esi),%ebx
		test   %ebx,%ebx
		setne  %al
		and    $0xff,%eax
		push   %eax
		test   %ebx,%ebx
		je     jump_fc179
		cmpl   $0x0,0x4c(%esi)
		je     jump_fc172
		mov    $0x3,%eax
		jmp    jump_fc188
	jump_fc172:
		mov    $0x1,%eax
		jmp    jump_fc188
	jump_fc179:
		cmpl   $0x0,0x4c(%esi)
		je     jump_fc186
		mov    $0x2,%eax
		jmp    jump_fc188
	jump_fc186:
		xor    %eax,%eax
	jump_fc188:
		push   %eax
		mov    0x38(%esi),%edx
		push   %edx
		call   AIL_set_sample_type
		add    $0xc,%esp
		mov    0x40(%esi),%ebx
		push   %ebx
		mov    0x38(%esi),%ecx
		push   %ecx
		call   AIL_set_sample_playback_rate
		add    $0x8,%esp
		push   %esi
		push   $0x0
		mov    0x38(%esi),%edi
		push   %edi
		call   AIL_set_sample_user_data
		add    $0xc,%esp
		push   $0x7f
		mov    0x38(%esi),%eax
		push   %eax
		call   AIL_set_sample_volume
		add    $0x8,%esp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (MSSLOWSOUNDPLAYED)
/*----------------------------------------------------------------*/
		push   %esi
		mov    0x8(%esp),%esi
		call   MSSLOWSOUNDCHECK
		call   *EXPORT_SYMBOL(_SmackTimerReadAddr)
		mov    $0x3e8,%ebx
		mov    %eax,%ecx
		mov    0x6c(%esi),%edx
		mov    0x14(%esi),%eax
		sub    %edx,%ecx
		mul    %ecx
		div    %ebx
		mov    0x68(%esi),%ebx
		cmp    %ebx,%eax
		jbe    jump_fc1fc
		mov    %ebx,%eax
	jump_fc1fc:
		add    0x50(%esi),%eax
		pop    %esi
		ret    $0x4


/*----------------------------------------------------------------*/
GLOBAL_FUNC (MSSLOWSOUNDCHECK)	/* 0xfc210 */
/*----------------------------------------------------------------*/
		push   %esi
		push   %edi
		push   %ebp
		mov    _fss,%esi
		test   %esi,%esi
		je     jump_fc2ef
	jump_fc221:
		mov    0x10(%esi),%eax
		cmp    0x64(%esi),%eax
		ja     jump_fc23c
		cmpl   $0x0,0x44(%esi)
		je     jump_fc2e4
		cmp    $0x3,%eax
		jbe    jump_fc2e4
	jump_fc23c:
		mov    0x38(%esi),%edi
		push   %edi
		call   AIL_sample_buffer_ready
		add    $0x4,%esp
		mov    %eax,%edi
		mov    %eax,%ebp
		cmp    $0x1,%edi
		ja     jump_fc2e4
		mov    0x10(%esi),%edi
		mov    0x3c(%esi),%eax
		and    $0xfffffffc,%di
		cmp    %eax,%edi
		jbe    jump_fc269
		mov    %eax,%edi
		and    $0xfffffffc,%di
	jump_fc269:
		mov    0x54(%esi,%ebp,4),%edx
		push   %edx
		mov    0x8(%esi),%ebx
		push   %ebx
		mov    (%esi),%ecx
		push   %ecx
		mov    0x4(%esi),%eax
		push   %eax
		push   %edi
		call   SMACKWRAPCOPY
		mov    0x6c(%esi),%edx
		mov    %eax,0x8(%esi)
		test   %edx,%edx
		je     jump_fc2a6
		mov    0x38(%esi),%eax
		call   func_fc300
		test   %al,%al
		jne    jump_fc2a6
		mov    0x38(%esi),%ebx
		push   %ebx
		call   AIL_sample_status
		add    $0x4,%esp
		cmp    $0x2,%eax
		jne    jump_fc2c1
	jump_fc2a6:
		call   *EXPORT_SYMBOL(_SmackTimerReadAddr)
		mov    %eax,0x6c(%esi)
		mov    0x50(%esi),%ecx
		mov    0x68(%esi),%eax
		movl   $0x0,0x68(%esi)
		add    %eax,%ecx
		mov    %ecx,0x50(%esi)
	jump_fc2c1:
		push   %edi
		mov    0x54(%esi,%ebp,4),%eax
		push   %eax
		push   %ebp
		mov    0x38(%esi),%edx
		push   %edx
		call   AIL_load_sample_buffer
		add    $0x10,%esp
		mov    0x68(%esi),%ebx
		mov    0x10(%esi),%ecx
		add    %edi,%ebx
		sub    %edi,%ecx
		mov    %ebx,0x68(%esi)
		mov    %ecx,0x10(%esi)
	jump_fc2e4:
		mov    0x34(%esi),%esi
		test   %esi,%esi
		jne    jump_fc221
	jump_fc2ef:
		pop    %ebp
		pop    %edi
		pop    %esi
		ret


/*----------------------------------------------------------------*/
func_fc300:
/*----------------------------------------------------------------*/
		push   %edx
		cmpl   $0x0,0x2c(%eax)
		jl     jump_fc31e
		mov    0x10(%eax),%edx
		cmp    0x18(%eax),%edx
		jne    jump_fc31e
		mov    0x14(%eax),%edx
		cmp    0x1c(%eax),%edx
		jne    jump_fc31e
		mov    $0x1,%eax
		pop    %edx
		ret
	jump_fc31e:
		xor    %eax,%eax
		pop    %edx
		ret

.section .rodata

data_15f7dc:
		.string "SB16.DIG"
		.align	4
data_15f7e8:
		.string "SBPRO.DIG"
		.align	4
data_15f7f4:
		.string "SBLASTER.DIG"
		.align	4

.data

_fss:	/* 1592cc */
		.long	0x0
GLOBAL (SmackMSSDigDriver)
		.long	0x0
MSSLiteInit:
		.long	0x0
data_1592d8:
		.long	0x0
data_1592dc: /* 0x1592dc */
		.long	0x0

data_1e86b0: /* 0x1e86b0 */
		.long	0x0
data_1e86b4:
		.fill   0x8
/* Smacker fn ptrs
 */
GLOBAL (_LowSoundVolPanAddr)
		.long	0x0
GLOBAL (_LowUnk_1e86c0)
		.fill   0x10
GLOBAL (_LowSoundPurgeAddr)
		.long	0x0
GLOBAL (_LowSoundCheckAddr)	/* 1e86d4 */
		.long	0x0
GLOBAL (_LowSoundPlayedAddr)
		.long	0x0
GLOBAL (_LowSoundCloseAddr)
		.long	0x0

