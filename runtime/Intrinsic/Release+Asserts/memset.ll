; ModuleID = 'memset.c'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32"
target triple = "i386-pc-linux-gnu"

define i8* @memset(i8* %dst, i32 %s, i32 %count) nounwind {
entry:
  tail call void @llvm.dbg.value(metadata !{i8* %dst}, i64 0, metadata !10), !dbg !17
  tail call void @llvm.dbg.value(metadata !{i32 %s}, i64 0, metadata !11), !dbg !17
  tail call void @llvm.dbg.value(metadata !{i32 %count}, i64 0, metadata !12), !dbg !17
  tail call void @llvm.dbg.value(metadata !{i8* %dst}, i64 0, metadata !13), !dbg !18
  tail call void @llvm.dbg.value(metadata !{null}, i64 0, metadata !12), !dbg !19
  %0 = icmp eq i32 %count, 0
  br i1 %0, label %bb2, label %bb.nph, !dbg !19

bb.nph:                                           ; preds = %entry
  %1 = trunc i32 %s to i8, !dbg !20
  br label %bb

bb:                                               ; preds = %bb.nph, %bb
  %indvar = phi i32 [ 0, %bb.nph ], [ %indvar.next, %bb ]
  %a.04 = getelementptr i8* %dst, i32 %indvar
  store i8 %1, i8* %a.04, align 1, !dbg !20
  %indvar.next = add i32 %indvar, 1
  %exitcond = icmp eq i32 %indvar.next, %count
  br i1 %exitcond, label %bb2, label %bb, !dbg !19

bb2:                                              ; preds = %bb, %entry
  ret i8* %dst, !dbg !21
}

declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

!llvm.dbg.sp = !{!0}
!llvm.dbg.lv.memset = !{!10, !11, !12, !13}

!0 = metadata !{i32 524334, i32 0, metadata !1, metadata !"memset", metadata !"memset", metadata !"memset", metadata !1, i32 12, metadata !3, i1 false, i1 true, i32 0, i32 0, null, i1 false, i1 true, i8* (i8*, i32, i32)* @memset} ; [ DW_TAG_subprogram ]
!1 = metadata !{i32 524329, metadata !"memset.c", metadata !"/home/xuanji/klee-nush/runtime/Intrinsic/", metadata !2} ; [ DW_TAG_file_type ]
!2 = metadata !{i32 524305, i32 0, i32 1, metadata !"memset.c", metadata !"/home/xuanji/klee-nush/runtime/Intrinsic/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.8)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!3 = metadata !{i32 524309, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !4, i32 0, null} ; [ DW_TAG_subroutine_type ]
!4 = metadata !{metadata !5, metadata !5, metadata !6, metadata !7}
!5 = metadata !{i32 524303, metadata !1, metadata !"", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ]
!6 = metadata !{i32 524324, metadata !1, metadata !"int", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!7 = metadata !{i32 524310, metadata !8, metadata !"size_t", metadata !8, i32 326, i64 0, i64 0, i64 0, i32 0, metadata !9} ; [ DW_TAG_typedef ]
!8 = metadata !{i32 524329, metadata !"stddef.h", metadata !"/home/xuanji/llvm-gcc-4.2-2.8-i686-linux/bin/../lib/gcc/i686-pc-linux-gnu/4.2.1/include", metadata !2} ; [ DW_TAG_file_type ]
!9 = metadata !{i32 524324, metadata !1, metadata !"unsigned int", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!10 = metadata !{i32 524545, metadata !0, metadata !"dst", metadata !1, i32 12, metadata !5} ; [ DW_TAG_arg_variable ]
!11 = metadata !{i32 524545, metadata !0, metadata !"s", metadata !1, i32 12, metadata !6} ; [ DW_TAG_arg_variable ]
!12 = metadata !{i32 524545, metadata !0, metadata !"count", metadata !1, i32 12, metadata !7} ; [ DW_TAG_arg_variable ]
!13 = metadata !{i32 524544, metadata !14, metadata !"a", metadata !1, i32 13, metadata !15} ; [ DW_TAG_auto_variable ]
!14 = metadata !{i32 524299, metadata !0, i32 12, i32 0, metadata !1, i32 0} ; [ DW_TAG_lexical_block ]
!15 = metadata !{i32 524303, metadata !1, metadata !"", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, metadata !16} ; [ DW_TAG_pointer_type ]
!16 = metadata !{i32 524324, metadata !1, metadata !"char", metadata !1, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!17 = metadata !{i32 12, i32 0, metadata !0, null}
!18 = metadata !{i32 13, i32 0, metadata !14, null}
!19 = metadata !{i32 14, i32 0, metadata !14, null}
!20 = metadata !{i32 15, i32 0, metadata !14, null}
!21 = metadata !{i32 16, i32 0, metadata !14, null}
