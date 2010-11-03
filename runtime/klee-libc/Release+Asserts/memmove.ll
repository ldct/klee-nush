; ModuleID = 'memmove.c'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32"
target triple = "i386-pc-linux-gnu"

define i8* @memmove(i8* %dst, i8* %src, i32 %count) nounwind {
entry:
  tail call void @llvm.dbg.value(metadata !{i8* %dst}, i64 0, metadata !9), !dbg !19
  tail call void @llvm.dbg.value(metadata !{i8* %src}, i64 0, metadata !10), !dbg !19
  tail call void @llvm.dbg.value(metadata !{i32 %count}, i64 0, metadata !11), !dbg !19
  tail call void @llvm.dbg.value(metadata !{i8* %dst}, i64 0, metadata !12), !dbg !20
  tail call void @llvm.dbg.value(metadata !{i8* %src}, i64 0, metadata !16), !dbg !21
  %0 = icmp eq i8* %src, %dst, !dbg !22
  br i1 %0, label %bb8, label %bb1, !dbg !22

bb1:                                              ; preds = %entry
  %1 = icmp ugt i8* %src, %dst, !dbg !23
  %2 = icmp eq i32 %count, 0
  br i1 %1, label %bb3.preheader, label %bb4, !dbg !23

bb3.preheader:                                    ; preds = %bb1
  tail call void @llvm.dbg.value(metadata !{null}, i64 0, metadata !11), !dbg !24
  br i1 %2, label %bb8, label %bb2, !dbg !24

bb2:                                              ; preds = %bb3.preheader, %bb2
  %indvar17 = phi i32 [ %indvar.next18, %bb2 ], [ 0, %bb3.preheader ]
  %b.014 = getelementptr i8* %src, i32 %indvar17
  %a.013 = getelementptr i8* %dst, i32 %indvar17
  %3 = load i8* %b.014, align 1, !dbg !24
  store i8 %3, i8* %a.013, align 1, !dbg !24
  %indvar.next18 = add i32 %indvar17, 1
  %exitcond19 = icmp eq i32 %indvar.next18, %count
  br i1 %exitcond19, label %bb8, label %bb2, !dbg !24

bb4:                                              ; preds = %bb1
  tail call void @llvm.dbg.value(metadata !{null}, i64 0, metadata !12), !dbg !25
  tail call void @llvm.dbg.value(metadata !{null}, i64 0, metadata !16), !dbg !26
  tail call void @llvm.dbg.value(metadata !{null}, i64 0, metadata !11), !dbg !27
  br i1 %2, label %bb8, label %bb.nph, !dbg !27

bb.nph:                                           ; preds = %bb4
  %4 = add i32 %count, -1
  br label %bb5

bb5:                                              ; preds = %bb.nph, %bb5
  %indvar = phi i32 [ 0, %bb.nph ], [ %indvar.next, %bb5 ]
  %tmp16 = sub i32 %4, %indvar
  %b.111 = getelementptr i8* %src, i32 %tmp16
  %a.110 = getelementptr i8* %dst, i32 %tmp16
  %5 = load i8* %b.111, align 1, !dbg !27
  store i8 %5, i8* %a.110, align 1, !dbg !27
  %indvar.next = add i32 %indvar, 1
  %exitcond = icmp eq i32 %indvar.next, %count
  br i1 %exitcond, label %bb8, label %bb5, !dbg !27

bb8:                                              ; preds = %bb3.preheader, %bb2, %bb4, %bb5, %entry
  ret i8* %dst, !dbg !28
}

declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

!llvm.dbg.sp = !{!0}
!llvm.dbg.lv.memmove = !{!9, !10, !11, !12, !16}

!0 = metadata !{i32 524334, i32 0, metadata !1, metadata !"memmove", metadata !"memmove", metadata !"memmove", metadata !1, i32 12, metadata !3, i1 false, i1 true, i32 0, i32 0, null, i1 false, i1 true, i8* (i8*, i8*, i32)* @memmove} ; [ DW_TAG_subprogram ]
!1 = metadata !{i32 524329, metadata !"memmove.c", metadata !"/home/xuanji/klee-nush/runtime/klee-libc/", metadata !2} ; [ DW_TAG_file_type ]
!2 = metadata !{i32 524305, i32 0, i32 1, metadata !"memmove.c", metadata !"/home/xuanji/klee-nush/runtime/klee-libc/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.8)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!3 = metadata !{i32 524309, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !4, i32 0, null} ; [ DW_TAG_subroutine_type ]
!4 = metadata !{metadata !5, metadata !5, metadata !5, metadata !6}
!5 = metadata !{i32 524303, metadata !1, metadata !"", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, null} ; [ DW_TAG_pointer_type ]
!6 = metadata !{i32 524310, metadata !7, metadata !"size_t", metadata !7, i32 326, i64 0, i64 0, i64 0, i32 0, metadata !8} ; [ DW_TAG_typedef ]
!7 = metadata !{i32 524329, metadata !"stddef.h", metadata !"/home/xuanji/llvm-gcc-4.2-2.8-i686-linux/bin/../lib/gcc/i686-pc-linux-gnu/4.2.1/include", metadata !2} ; [ DW_TAG_file_type ]
!8 = metadata !{i32 524324, metadata !1, metadata !"unsigned int", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!9 = metadata !{i32 524545, metadata !0, metadata !"dst", metadata !1, i32 12, metadata !5} ; [ DW_TAG_arg_variable ]
!10 = metadata !{i32 524545, metadata !0, metadata !"src", metadata !1, i32 12, metadata !5} ; [ DW_TAG_arg_variable ]
!11 = metadata !{i32 524545, metadata !0, metadata !"count", metadata !1, i32 12, metadata !6} ; [ DW_TAG_arg_variable ]
!12 = metadata !{i32 524544, metadata !13, metadata !"a", metadata !1, i32 13, metadata !14} ; [ DW_TAG_auto_variable ]
!13 = metadata !{i32 524299, metadata !0, i32 12, i32 0, metadata !1, i32 0} ; [ DW_TAG_lexical_block ]
!14 = metadata !{i32 524303, metadata !1, metadata !"", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, metadata !15} ; [ DW_TAG_pointer_type ]
!15 = metadata !{i32 524324, metadata !1, metadata !"char", metadata !1, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!16 = metadata !{i32 524544, metadata !13, metadata !"b", metadata !1, i32 14, metadata !17} ; [ DW_TAG_auto_variable ]
!17 = metadata !{i32 524303, metadata !1, metadata !"", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, metadata !18} ; [ DW_TAG_pointer_type ]
!18 = metadata !{i32 524326, metadata !1, metadata !"", metadata !1, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !15} ; [ DW_TAG_const_type ]
!19 = metadata !{i32 12, i32 0, metadata !0, null}
!20 = metadata !{i32 13, i32 0, metadata !13, null}
!21 = metadata !{i32 14, i32 0, metadata !13, null}
!22 = metadata !{i32 16, i32 0, metadata !13, null}
!23 = metadata !{i32 19, i32 0, metadata !13, null}
!24 = metadata !{i32 20, i32 0, metadata !13, null}
!25 = metadata !{i32 22, i32 0, metadata !13, null}
!26 = metadata !{i32 23, i32 0, metadata !13, null}
!27 = metadata !{i32 24, i32 0, metadata !13, null}
!28 = metadata !{i32 17, i32 0, metadata !13, null}
