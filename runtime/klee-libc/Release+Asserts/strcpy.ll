; ModuleID = 'strcpy.c'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32"
target triple = "i386-pc-linux-gnu"

define i8* @strcpy(i8* %to, i8* nocapture %from) nounwind {
entry:
  tail call void @llvm.dbg.value(metadata !{i8* %to}, i64 0, metadata !9), !dbg !13
  tail call void @llvm.dbg.value(metadata !{i8* %from}, i64 0, metadata !10), !dbg !13
  tail call void @llvm.dbg.value(metadata !{i8* %to}, i64 0, metadata !11), !dbg !14
  %0 = load i8* %from, align 1, !dbg !15
  store i8 %0, i8* %to, align 1, !dbg !15
  %1 = icmp eq i8 %0, 0, !dbg !15
  br i1 %1, label %bb2, label %bb, !dbg !15

bb:                                               ; preds = %entry, %bb
  %indvar = phi i32 [ %tmp, %bb ], [ 0, %entry ]
  %tmp = add i32 %indvar, 1
  %scevgep6 = getelementptr i8* %from, i32 %tmp
  %scevgep = getelementptr i8* %to, i32 %tmp
  %2 = load i8* %scevgep6, align 1, !dbg !15
  store i8 %2, i8* %scevgep, align 1, !dbg !15
  %3 = icmp eq i8 %2, 0, !dbg !15
  br i1 %3, label %bb2, label %bb, !dbg !15

bb2:                                              ; preds = %bb, %entry
  ret i8* %to, !dbg !16
}

declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

!llvm.dbg.sp = !{!0}
!llvm.dbg.lv.strcpy = !{!9, !10, !11}

!0 = metadata !{i32 524334, i32 0, metadata !1, metadata !"strcpy", metadata !"strcpy", metadata !"strcpy", metadata !1, i32 10, metadata !3, i1 false, i1 true, i32 0, i32 0, null, i1 false, i1 true, i8* (i8*, i8*)* @strcpy} ; [ DW_TAG_subprogram ]
!1 = metadata !{i32 524329, metadata !"strcpy.c", metadata !"/home/xuanji/klee-nush/runtime/klee-libc/", metadata !2} ; [ DW_TAG_file_type ]
!2 = metadata !{i32 524305, i32 0, i32 1, metadata !"strcpy.c", metadata !"/home/xuanji/klee-nush/runtime/klee-libc/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.8)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!3 = metadata !{i32 524309, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !4, i32 0, null} ; [ DW_TAG_subroutine_type ]
!4 = metadata !{metadata !5, metadata !5, metadata !7}
!5 = metadata !{i32 524303, metadata !1, metadata !"", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, metadata !6} ; [ DW_TAG_pointer_type ]
!6 = metadata !{i32 524324, metadata !1, metadata !"char", metadata !1, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!7 = metadata !{i32 524303, metadata !1, metadata !"", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, metadata !8} ; [ DW_TAG_pointer_type ]
!8 = metadata !{i32 524326, metadata !1, metadata !"", metadata !1, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !6} ; [ DW_TAG_const_type ]
!9 = metadata !{i32 524545, metadata !0, metadata !"to", metadata !1, i32 10, metadata !5} ; [ DW_TAG_arg_variable ]
!10 = metadata !{i32 524545, metadata !0, metadata !"from", metadata !1, i32 10, metadata !7} ; [ DW_TAG_arg_variable ]
!11 = metadata !{i32 524544, metadata !12, metadata !"start", metadata !1, i32 11, metadata !5} ; [ DW_TAG_auto_variable ]
!12 = metadata !{i32 524299, metadata !0, i32 10, i32 0, metadata !1, i32 0} ; [ DW_TAG_lexical_block ]
!13 = metadata !{i32 10, i32 0, metadata !0, null}
!14 = metadata !{i32 11, i32 0, metadata !12, null}
!15 = metadata !{i32 13, i32 0, metadata !12, null}
!16 = metadata !{i32 16, i32 0, metadata !12, null}
