; ModuleID = 'putchar.c'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32"
target triple = "i386-pc-linux-gnu"

define i32 @putchar(i32 %c) nounwind {
entry:
  %x = alloca i8, align 1
  call void @llvm.dbg.value(metadata !{i32 %c}, i64 0, metadata !6), !dbg !10
  call void @llvm.dbg.declare(metadata !{i8* %x}, metadata !7), !dbg !11
  %0 = trunc i32 %c to i8, !dbg !11
  store i8 %0, i8* %x, align 1, !dbg !11
  %1 = call i32 @write(i32 1, i8* %x, i32 1) nounwind, !dbg !12
  ret i32 1, !dbg !13
}

declare void @llvm.dbg.declare(metadata, metadata) nounwind readnone

declare i32 @write(i32, i8* nocapture, i32)

declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

!llvm.dbg.sp = !{!0}
!llvm.dbg.lv.putchar = !{!6, !7}

!0 = metadata !{i32 524334, i32 0, metadata !1, metadata !"putchar", metadata !"putchar", metadata !"putchar", metadata !1, i32 16, metadata !3, i1 false, i1 true, i32 0, i32 0, null, i1 false, i1 true, i32 (i32)* @putchar} ; [ DW_TAG_subprogram ]
!1 = metadata !{i32 524329, metadata !"putchar.c", metadata !"/home/xuanji/klee-nush/runtime/klee-libc/", metadata !2} ; [ DW_TAG_file_type ]
!2 = metadata !{i32 524305, i32 0, i32 1, metadata !"putchar.c", metadata !"/home/xuanji/klee-nush/runtime/klee-libc/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.8)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!3 = metadata !{i32 524309, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !4, i32 0, null} ; [ DW_TAG_subroutine_type ]
!4 = metadata !{metadata !5, metadata !5}
!5 = metadata !{i32 524324, metadata !1, metadata !"int", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!6 = metadata !{i32 524545, metadata !0, metadata !"c", metadata !1, i32 16, metadata !5} ; [ DW_TAG_arg_variable ]
!7 = metadata !{i32 524544, metadata !8, metadata !"x", metadata !1, i32 17, metadata !9} ; [ DW_TAG_auto_variable ]
!8 = metadata !{i32 524299, metadata !0, i32 16, i32 0, metadata !1, i32 0} ; [ DW_TAG_lexical_block ]
!9 = metadata !{i32 524324, metadata !1, metadata !"char", metadata !1, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!10 = metadata !{i32 16, i32 0, metadata !0, null}
!11 = metadata !{i32 17, i32 0, metadata !8, null}
!12 = metadata !{i32 18, i32 0, metadata !8, null}
!13 = metadata !{i32 19, i32 0, metadata !8, null}
