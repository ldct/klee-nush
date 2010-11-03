; ModuleID = 'strtoul.c'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32"
target triple = "i386-pc-linux-gnu"

define i32 @strtoul(i8* %nptr, i8** %endptr, i32 %base) nounwind {
entry:
  tail call void @llvm.dbg.value(metadata !{i8* %nptr}, i64 0, metadata !12), !dbg !23
  tail call void @llvm.dbg.value(metadata !{i8** %endptr}, i64 0, metadata !13), !dbg !23
  tail call void @llvm.dbg.value(metadata !{i32 %base}, i64 0, metadata !14), !dbg !23
  tail call void @llvm.dbg.value(metadata !{i8* %nptr}, i64 0, metadata !15), !dbg !24
  %0 = tail call i16** @__ctype_b_loc() nounwind readnone, !dbg !25
  %1 = load i16** %0, align 4, !dbg !25
  br label %bb, !dbg !24

bb:                                               ; preds = %bb, %entry
  %indvar57 = phi i32 [ %tmp59, %bb ], [ 0, %entry ]
  %s.0 = getelementptr i8* %nptr, i32 %indvar57
  %tmp59 = add i32 %indvar57, 1
  %scevgep = getelementptr i8* %nptr, i32 %tmp59
  %2 = load i8* %s.0, align 1, !dbg !26
  %3 = zext i8 %2 to i32, !dbg !25
  %4 = getelementptr inbounds i16* %1, i32 %3, !dbg !25
  %5 = load i16* %4, align 2, !dbg !25
  %6 = zext i16 %5 to i32, !dbg !25
  %7 = and i32 %6, 8192, !dbg !25
  %8 = icmp eq i32 %7, 0, !dbg !25
  br i1 %8, label %bb1, label %bb, !dbg !25

bb1:                                              ; preds = %bb
  tail call void @llvm.dbg.value(metadata !{i8 %2}, i64 0, metadata !18), !dbg !26
  tail call void @llvm.dbg.value(metadata !{null}, i64 0, metadata !15), !dbg !26
  switch i8 %2, label %bb5 [
    i8 45, label %bb2
    i8 43, label %bb4
  ]

bb2:                                              ; preds = %bb1
  tail call void @llvm.dbg.value(metadata !27, i64 0, metadata !20), !dbg !28
  %9 = load i8* %scevgep, align 1, !dbg !29
  tail call void @llvm.dbg.value(metadata !{i8 %9}, i64 0, metadata !18), !dbg !29
  %s.0.sum60 = add i32 %indvar57, 2
  %10 = getelementptr inbounds i8* %nptr, i32 %s.0.sum60, !dbg !29
  tail call void @llvm.dbg.value(metadata !{i8* %10}, i64 0, metadata !15), !dbg !29
  br label %bb5, !dbg !29

bb4:                                              ; preds = %bb1
  %11 = load i8* %scevgep, align 1, !dbg !30
  tail call void @llvm.dbg.value(metadata !{i8 %11}, i64 0, metadata !18), !dbg !30
  %s.0.sum = add i32 %indvar57, 2
  %12 = getelementptr inbounds i8* %nptr, i32 %s.0.sum, !dbg !30
  tail call void @llvm.dbg.value(metadata !{i8* %12}, i64 0, metadata !15), !dbg !30
  br label %bb5, !dbg !30

bb5:                                              ; preds = %bb1, %bb4, %bb2
  %s.1 = phi i8* [ %10, %bb2 ], [ %12, %bb4 ], [ %scevgep, %bb1 ]
  %c.0 = phi i8 [ %9, %bb2 ], [ %11, %bb4 ], [ %2, %bb1 ]
  %neg.0 = phi i32 [ 1, %bb2 ], [ 0, %bb4 ], [ 0, %bb1 ]
  %13 = icmp eq i32 %base, 0, !dbg !31
  %14 = icmp eq i32 %base, 16, !dbg !31
  %15 = or i1 %13, %14, !dbg !31
  %16 = icmp eq i8 %c.0, 48, !dbg !31
  %or.cond = and i1 %15, %16
  br i1 %or.cond, label %bb9, label %bb14, !dbg !31

bb9:                                              ; preds = %bb5
  %17 = load i8* %s.1, align 1, !dbg !31
  switch i8 %17, label %bb14 [
    i8 88, label %bb19.thread
    i8 120, label %bb19.thread
  ]

bb19.thread:                                      ; preds = %bb9, %bb9
  %18 = getelementptr inbounds i8* %s.1, i32 1, !dbg !32
  %19 = load i8* %18, align 1, !dbg !32
  tail call void @llvm.dbg.value(metadata !{i8 %19}, i64 0, metadata !18), !dbg !32
  %20 = getelementptr inbounds i8* %s.1, i32 2, !dbg !33
  tail call void @llvm.dbg.value(metadata !{i8* %20}, i64 0, metadata !15), !dbg !33
  tail call void @llvm.dbg.value(metadata !34, i64 0, metadata !14), !dbg !35
  tail call void @llvm.dbg.value(metadata !36, i64 0, metadata !21), !dbg !37
  tail call void @llvm.dbg.value(metadata !36, i64 0, metadata !17), !dbg !37
  br label %bb20

bb14:                                             ; preds = %bb9, %bb5
  br i1 %13, label %bb15, label %bb19, !dbg !38

bb15:                                             ; preds = %bb14
  %iftmp.1.0 = select i1 %16, i32 8, i32 10
  tail call void @llvm.dbg.value(metadata !{i32 %iftmp.1.0}, i64 0, metadata !14), !dbg !39
  br label %bb19, !dbg !39

bb19:                                             ; preds = %bb15, %bb14
  %base_addr.1 = phi i32 [ %iftmp.1.0, %bb15 ], [ %base, %bb14 ]
  tail call void @llvm.dbg.value(metadata !36, i64 0, metadata !21), !dbg !37
  tail call void @llvm.dbg.value(metadata !36, i64 0, metadata !17), !dbg !37
  %21 = add i32 %base_addr.1, -2
  %22 = icmp ugt i32 %21, 34, !dbg !40
  br i1 %22, label %noconv, label %bb20, !dbg !40

bb20:                                             ; preds = %bb19.thread, %bb19
  %base_addr.154 = phi i32 [ 16, %bb19.thread ], [ %base_addr.1, %bb19 ]
  %s.25053 = phi i8* [ %20, %bb19.thread ], [ %s.1, %bb19 ]
  %c.15152 = phi i8 [ %19, %bb19.thread ], [ %c.0, %bb19 ]
  %23 = udiv i32 -1, %base_addr.154, !dbg !41
  tail call void @llvm.dbg.value(metadata !{i32 %23}, i64 0, metadata !19), !dbg !41
  %24 = urem i32 -1, %base_addr.154, !dbg !42
  tail call void @llvm.dbg.value(metadata !{i32 %24}, i64 0, metadata !22), !dbg !42
  br label %bb21, !dbg !42

bb21:                                             ; preds = %bb36, %bb20
  %indvar = phi i32 [ %indvar.next, %bb36 ], [ 0, %bb20 ]
  %acc.1 = phi i32 [ %acc.0, %bb36 ], [ 0, %bb20 ]
  %c.3 = phi i8 [ %42, %bb36 ], [ %c.15152, %bb20 ]
  %any.1 = phi i32 [ %any.0, %bb36 ], [ 0, %bb20 ]
  %s.3 = getelementptr i8* %s.25053, i32 %indvar
  %25 = add i8 %c.3, -48
  %26 = icmp ult i8 %25, 10
  br i1 %26, label %bb27, label %bb23, !dbg !43

bb23:                                             ; preds = %bb21
  %27 = add i8 %c.3, -65
  %28 = icmp ult i8 %27, 26
  br i1 %28, label %bb24, label %bb25, !dbg !44

bb24:                                             ; preds = %bb23
  %29 = add i8 %c.3, -55
  br label %bb27, !dbg !45

bb25:                                             ; preds = %bb23
  %30 = add i8 %c.3, -97
  %31 = icmp ult i8 %30, 26
  br i1 %31, label %bb26, label %bb37, !dbg !46

bb26:                                             ; preds = %bb25
  %32 = add i8 %c.3, -87
  br label %bb27, !dbg !47

bb27:                                             ; preds = %bb21, %bb26, %bb24
  %c.2 = phi i8 [ %29, %bb24 ], [ %32, %bb26 ], [ %25, %bb21 ]
  %33 = sext i8 %c.2 to i32, !dbg !48
  %34 = icmp slt i32 %33, %base_addr.154, !dbg !48
  br i1 %34, label %bb28, label %bb37, !dbg !48

bb28:                                             ; preds = %bb27
  %35 = icmp ugt i32 %acc.1, %23, !dbg !49
  %any.1.lobit = lshr i32 %any.1, 31
  %tmp = trunc i32 %any.1.lobit to i8
  %36 = zext i1 %35 to i8
  %37 = or i8 %tmp, %36
  %toBool31 = icmp eq i8 %37, 0, !dbg !49
  br i1 %toBool31, label %bb32, label %bb36, !dbg !49

bb32:                                             ; preds = %bb28
  %38 = icmp eq i32 %acc.1, %23, !dbg !49
  %39 = icmp sgt i32 %33, %24, !dbg !49
  %or.cond61 = and i1 %38, %39
  br i1 %or.cond61, label %bb36, label %bb35, !dbg !49

bb35:                                             ; preds = %bb32
  %40 = mul i32 %acc.1, %base_addr.154, !dbg !50
  %41 = add i32 %33, %40, !dbg !51
  br label %bb36, !dbg !51

bb36:                                             ; preds = %bb32, %bb28, %bb35
  %acc.0 = phi i32 [ %41, %bb35 ], [ %acc.1, %bb28 ], [ %acc.1, %bb32 ]
  %any.0 = phi i32 [ 1, %bb35 ], [ -1, %bb28 ], [ -1, %bb32 ]
  %42 = load i8* %s.3, align 1, !dbg !52
  %indvar.next = add i32 %indvar, 1
  br label %bb21, !dbg !52

bb37:                                             ; preds = %bb27, %bb25
  %43 = icmp slt i32 %any.1, 0, !dbg !53
  br i1 %43, label %bb38, label %bb39, !dbg !53

bb38:                                             ; preds = %bb37
  tail call void @llvm.dbg.value(metadata !54, i64 0, metadata !17), !dbg !55
  %44 = tail call i32* @__errno_location() nounwind readnone, !dbg !56
  store i32 34, i32* %44, align 4, !dbg !56
  br label %bb42, !dbg !56

bb39:                                             ; preds = %bb37
  %45 = icmp eq i32 %any.1, 0, !dbg !57
  br i1 %45, label %noconv, label %bb40, !dbg !57

noconv:                                           ; preds = %bb39, %bb19
  %s.4 = phi i8* [ %s.1, %bb19 ], [ %s.3, %bb39 ]
  %acc.2 = phi i32 [ 0, %bb19 ], [ %acc.1, %bb39 ]
  %46 = tail call i32* @__errno_location() nounwind readnone, !dbg !58
  store i32 22, i32* %46, align 4, !dbg !58
  br label %bb42, !dbg !58

bb40:                                             ; preds = %bb39
  %47 = sub i32 0, %acc.1, !dbg !59
  %48 = icmp eq i32 %neg.0, 0, !dbg !60
  %acc.1. = select i1 %48, i32 %acc.1, i32 %47
  br label %bb42, !dbg !60

bb42:                                             ; preds = %bb40, %noconv, %bb38
  %s.5 = phi i8* [ %s.4, %noconv ], [ %s.3, %bb38 ], [ %s.3, %bb40 ]
  %acc.3 = phi i32 [ %acc.2, %noconv ], [ -1, %bb38 ], [ %acc.1., %bb40 ]
  %any.3 = phi i32 [ 0, %noconv ], [ -1, %bb38 ], [ %any.1, %bb40 ]
  %49 = icmp eq i8** %endptr, null, !dbg !61
  br i1 %49, label %bb47, label %bb43, !dbg !61

bb43:                                             ; preds = %bb42
  %50 = icmp eq i32 %any.3, 0, !dbg !62
  br i1 %50, label %bb46, label %bb44, !dbg !62

bb44:                                             ; preds = %bb43
  %51 = getelementptr inbounds i8* %s.5, i32 -1, !dbg !62
  br label %bb46, !dbg !62

bb46:                                             ; preds = %bb43, %bb44
  %iftmp.12.0 = phi i8* [ %51, %bb44 ], [ %nptr, %bb43 ]
  store i8* %iftmp.12.0, i8** %endptr, align 4, !dbg !62
  ret i32 %acc.3, !dbg !63

bb47:                                             ; preds = %bb42
  ret i32 %acc.3, !dbg !63
}

declare i16** @__ctype_b_loc() nounwind readnone

declare i32* @__errno_location() nounwind readnone

declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

!llvm.dbg.sp = !{!0}
!llvm.dbg.lv.strtoul = !{!12, !13, !14, !15, !17, !18, !19, !20, !21, !22}

!0 = metadata !{i32 524334, i32 0, metadata !1, metadata !"strtoul", metadata !"strtoul", metadata !"strtoul", metadata !1, i32 47, metadata !3, i1 false, i1 true, i32 0, i32 0, null, i1 false, i1 true, i32 (i8*, i8**, i32)* @strtoul} ; [ DW_TAG_subprogram ]
!1 = metadata !{i32 524329, metadata !"strtoul.c", metadata !"/home/xuanji/klee-nush/runtime/klee-libc/", metadata !2} ; [ DW_TAG_file_type ]
!2 = metadata !{i32 524305, i32 0, i32 1, metadata !"strtoul.c", metadata !"/home/xuanji/klee-nush/runtime/klee-libc/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.8)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!3 = metadata !{i32 524309, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !4, i32 0, null} ; [ DW_TAG_subroutine_type ]
!4 = metadata !{metadata !5, metadata !6, metadata !9, metadata !11}
!5 = metadata !{i32 524324, metadata !1, metadata !"long unsigned int", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!6 = metadata !{i32 524303, metadata !1, metadata !"", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, metadata !7} ; [ DW_TAG_pointer_type ]
!7 = metadata !{i32 524326, metadata !1, metadata !"", metadata !1, i32 0, i64 8, i64 8, i64 0, i32 0, metadata !8} ; [ DW_TAG_const_type ]
!8 = metadata !{i32 524324, metadata !1, metadata !"char", metadata !1, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!9 = metadata !{i32 524303, metadata !1, metadata !"", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, metadata !10} ; [ DW_TAG_pointer_type ]
!10 = metadata !{i32 524303, metadata !1, metadata !"", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, metadata !8} ; [ DW_TAG_pointer_type ]
!11 = metadata !{i32 524324, metadata !1, metadata !"int", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!12 = metadata !{i32 524545, metadata !0, metadata !"nptr", metadata !1, i32 46, metadata !6} ; [ DW_TAG_arg_variable ]
!13 = metadata !{i32 524545, metadata !0, metadata !"endptr", metadata !1, i32 46, metadata !9} ; [ DW_TAG_arg_variable ]
!14 = metadata !{i32 524545, metadata !0, metadata !"base", metadata !1, i32 46, metadata !11} ; [ DW_TAG_arg_variable ]
!15 = metadata !{i32 524544, metadata !16, metadata !"s", metadata !1, i32 48, metadata !6} ; [ DW_TAG_auto_variable ]
!16 = metadata !{i32 524299, metadata !0, i32 47, i32 0, metadata !1, i32 0} ; [ DW_TAG_lexical_block ]
!17 = metadata !{i32 524544, metadata !16, metadata !"acc", metadata !1, i32 49, metadata !5} ; [ DW_TAG_auto_variable ]
!18 = metadata !{i32 524544, metadata !16, metadata !"c", metadata !1, i32 50, metadata !8} ; [ DW_TAG_auto_variable ]
!19 = metadata !{i32 524544, metadata !16, metadata !"cutoff", metadata !1, i32 51, metadata !5} ; [ DW_TAG_auto_variable ]
!20 = metadata !{i32 524544, metadata !16, metadata !"neg", metadata !1, i32 52, metadata !11} ; [ DW_TAG_auto_variable ]
!21 = metadata !{i32 524544, metadata !16, metadata !"any", metadata !1, i32 52, metadata !11} ; [ DW_TAG_auto_variable ]
!22 = metadata !{i32 524544, metadata !16, metadata !"cutlim", metadata !1, i32 52, metadata !11} ; [ DW_TAG_auto_variable ]
!23 = metadata !{i32 46, i32 0, metadata !0, null}
!24 = metadata !{i32 57, i32 0, metadata !16, null}
!25 = metadata !{i32 60, i32 0, metadata !16, null}
!26 = metadata !{i32 59, i32 0, metadata !16, null}
!27 = metadata !{i32 1}
!28 = metadata !{i32 62, i32 0, metadata !16, null}
!29 = metadata !{i32 63, i32 0, metadata !16, null}
!30 = metadata !{i32 67, i32 0, metadata !16, null}
!31 = metadata !{i32 69, i32 0, metadata !16, null}
!32 = metadata !{i32 71, i32 0, metadata !16, null}
!33 = metadata !{i32 72, i32 0, metadata !16, null}
!34 = metadata !{i32 16}
!35 = metadata !{i32 73, i32 0, metadata !16, null}
!36 = metadata !{i32 0}
!37 = metadata !{i32 77, i32 0, metadata !16, null}
!38 = metadata !{i32 75, i32 0, metadata !16, null}
!39 = metadata !{i32 76, i32 0, metadata !16, null}
!40 = metadata !{i32 78, i32 0, metadata !16, null}
!41 = metadata !{i32 81, i32 0, metadata !16, null}
!42 = metadata !{i32 82, i32 0, metadata !16, null}
!43 = metadata !{i32 84, i32 0, metadata !16, null}
!44 = metadata !{i32 86, i32 0, metadata !16, null}
!45 = metadata !{i32 87, i32 0, metadata !16, null}
!46 = metadata !{i32 88, i32 0, metadata !16, null}
!47 = metadata !{i32 89, i32 0, metadata !16, null}
!48 = metadata !{i32 92, i32 0, metadata !16, null}
!49 = metadata !{i32 94, i32 0, metadata !16, null}
!50 = metadata !{i32 98, i32 0, metadata !16, null}
!51 = metadata !{i32 99, i32 0, metadata !16, null}
!52 = metadata !{i32 83, i32 0, metadata !16, null}
!53 = metadata !{i32 102, i32 0, metadata !16, null}
!54 = metadata !{i32 -1}                          ; [ DW_TAG_hi_user ]
!55 = metadata !{i32 103, i32 0, metadata !16, null}
!56 = metadata !{i32 104, i32 0, metadata !16, null}
!57 = metadata !{i32 105, i32 0, metadata !16, null}
!58 = metadata !{i32 107, i32 0, metadata !16, null}
!59 = metadata !{i32 109, i32 0, metadata !16, null}
!60 = metadata !{i32 108, i32 0, metadata !16, null}
!61 = metadata !{i32 110, i32 0, metadata !16, null}
!62 = metadata !{i32 111, i32 0, metadata !16, null}
!63 = metadata !{i32 112, i32 0, metadata !16, null}
