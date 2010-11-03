; ModuleID = 'strncpy.c'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32"
target triple = "i386-pc-linux-gnu"

define i8* @strncpy(i8* %dst, i8* nocapture %src, i32 %n) nounwind {
entry:
  tail call void @llvm.dbg.value(metadata !{i8* %dst}, i64 0, metadata !12), !dbg !19
  tail call void @llvm.dbg.value(metadata !{i8* %src}, i64 0, metadata !13), !dbg !19
  tail call void @llvm.dbg.value(metadata !{i32 %n}, i64 0, metadata !14), !dbg !19
  %0 = icmp eq i32 %n, 0, !dbg !20
  br i1 %0, label %bb5, label %bb1.preheader, !dbg !20

bb1.preheader:                                    ; preds = %entry
  %tmp21 = add i32 %n, -1
  br label %bb1

bb1:                                              ; preds = %bb1.preheader, %bb4
  %1 = phi i32 [ 0, %bb1.preheader ], [ %indvar.next12, %bb4 ]
  %d.0 = getelementptr i8* %dst, i32 %1
  %s.0 = getelementptr i8* %src, i32 %1
  %2 = load i8* %s.0, align 1, !dbg !21
  store i8 %2, i8* %d.0, align 1, !dbg !21
  %3 = icmp eq i8 %2, 0, !dbg !21
  br i1 %3, label %bb3.preheader, label %bb4, !dbg !21

bb3.preheader:                                    ; preds = %bb1
  %n_addr.1 = sub i32 %n, %1
  tail call void @llvm.dbg.value(metadata !{null}, i64 0, metadata !15), !dbg !21
  tail call void @llvm.dbg.value(metadata !{null}, i64 0, metadata !18), !dbg !21
  tail call void @llvm.dbg.value(metadata !{null}, i64 0, metadata !14), !dbg !22
  %4 = icmp eq i32 %n_addr.1, 1
  br i1 %4, label %bb5, label %bb.nph, !dbg !22

bb.nph:                                           ; preds = %bb3.preheader
  %tmp14 = sub i32 %tmp21, %1
  %tmp15 = add i32 %1, 1
  br label %bb2

bb2:                                              ; preds = %bb.nph, %bb2
  %indvar = phi i32 [ 0, %bb.nph ], [ %indvar.next, %bb2 ]
  %tmp16 = add i32 %tmp15, %indvar
  %d.17 = getelementptr i8* %dst, i32 %tmp16
  store i8 0, i8* %d.17, align 1, !dbg !23
  %indvar.next = add i32 %indvar, 1
  %exitcond = icmp eq i32 %indvar.next, %tmp14
  br i1 %exitcond, label %bb5, label %bb2, !dbg !22

bb4:                                              ; preds = %bb1
  %5 = icmp eq i32 %tmp21, %1
  %indvar.next12 = add i32 %1, 1
  br i1 %5, label %bb5, label %bb1, !dbg !24

bb5:                                              ; preds = %bb4, %bb3.preheader, %bb2, %entry
  ret i8* %dst, !dbg !25
}

declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

!llvm.dbg.sp = !{!0}
!llvm.dbg.lv.strncpy = !{!12, !13, !14, !15, !18}

!0 = metadata !{i32 524334, i32 0, metadata !1, metadata !"strncpy", metadata !"strncpy", metadata !"strncpy", metadata !1, i32 43, metadata !3, i1 false, i1 true, i32 0, i32 0, null, i1 false, i1 true, i8* (i8*, i8*, i32)* @strncpy} ; [ DW_TAG_subprogram ]
!1 = metadata !{i32 524329, metadata !"strncpy.c", metadata !"/home/xuanji/klee-nush/runtime/klee-libc/", metadata !2} ; [ DW_TAG_file_type ]
!2 = metadata !{i32 524305, i32 0, i32 1, metadata !"strncpy.c", metadata !"/home/xuanji/klee-nush/runtime/klee-libc/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.8)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!3 = metadata !{i32 524309, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !4, i32 0, null} ; [ DW_TAG_subroutine_type ]
!4 = metadata !{metadata !5, metadata !5, metadata !7, metadata !9}
!5 = metadata !{i32 524303, metadata !1, metadata !"", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, metadata !6} ; [ DW_TAG_pointer_type ]
!6 = metadata !{i32 524324, metadata !1, metadata !"char", metadata !1, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!7 = metadata !{i32 524303, metadata !1, metadata !"", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, metadata !8} ; [ DW_TAG_pointer_type ]
!8 = metadata !{i32 524326, metadata !1, metadata !"", metadata !1, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !6} ; [ DW_TAG_const_type ]
!9 = metadata !{i32 524310, metadata !10, metadata !"size_t", metadata !10, i32 326, i64 0, i64 0, i64 0, i32 0, metadata !11} ; [ DW_TAG_typedef ]
!10 = metadata !{i32 524329, metadata !"stddef.h", metadata !"/home/xuanji/llvm-gcc-4.2-2.8-i686-linux/bin/../lib/gcc/i686-pc-linux-gnu/4.2.1/include", metadata !2} ; [ DW_TAG_file_type ]
!11 = metadata !{i32 524324, metadata !1, metadata !"unsigned int", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!12 = metadata !{i32 524545, metadata !0, metadata !"dst", metadata !1, i32 42, metadata !5} ; [ DW_TAG_arg_variable ]
!13 = metadata !{i32 524545, metadata !0, metadata !"src", metadata !1, i32 42, metadata !7} ; [ DW_TAG_arg_variable ]
!14 = metadata !{i32 524545, metadata !0, metadata !"n", metadata !1, i32 42, metadata !9} ; [ DW_TAG_arg_variable ]
!15 = metadata !{i32 524544, metadata !16, metadata !"d", metadata !1, i32 45, metadata !5} ; [ DW_TAG_auto_variable ]
!16 = metadata !{i32 524299, metadata !17, i32 43, i32 0, metadata !1, i32 1} ; [ DW_TAG_lexical_block ]
!17 = metadata !{i32 524299, metadata !0, i32 43, i32 0, metadata !1, i32 0} ; [ DW_TAG_lexical_block ]
!18 = metadata !{i32 524544, metadata !16, metadata !"s", metadata !1, i32 46, metadata !7} ; [ DW_TAG_auto_variable ]
!19 = metadata !{i32 42, i32 0, metadata !0, null}
!20 = metadata !{i32 44, i32 0, metadata !17, null}
!21 = metadata !{i32 49, i32 0, metadata !16, null}
!22 = metadata !{i32 51, i32 0, metadata !16, null}
!23 = metadata !{i32 52, i32 0, metadata !16, null}
!24 = metadata !{i32 55, i32 0, metadata !16, null}
!25 = metadata !{i32 57, i32 0, metadata !17, null}
