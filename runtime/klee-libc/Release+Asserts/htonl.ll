; ModuleID = 'htonl.c'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32"
target triple = "i386-pc-linux-gnu"

define zeroext i16 @htons(i16 zeroext %v) nounwind readnone {
entry:
  tail call void @llvm.dbg.value(metadata !{i16 %v}, i64 0, metadata !17), !dbg !21
  %0 = tail call i16 @llvm.bswap.i16(i16 %v)
  ret i16 %0, !dbg !22
}

declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

declare i16 @llvm.bswap.i16(i16) nounwind readnone

define i32 @htonl(i32 %v) nounwind readnone {
entry:
  tail call void @llvm.dbg.value(metadata !{i32 %v}, i64 0, metadata !18), !dbg !24
  %0 = lshr i32 %v, 16, !dbg !25
  %1 = trunc i32 %0 to i16, !dbg !25
  tail call void @llvm.dbg.value(metadata !{i16 %1}, i64 0, metadata !17) nounwind, !dbg !27
  %2 = tail call i16 @llvm.bswap.i16(i16 %1) nounwind
  %3 = zext i16 %2 to i32, !dbg !25
  %4 = trunc i32 %v to i16, !dbg !25
  tail call void @llvm.dbg.value(metadata !{i16 %4}, i64 0, metadata !17) nounwind, !dbg !27
  %5 = tail call i16 @llvm.bswap.i16(i16 %4) nounwind
  %6 = zext i16 %5 to i32, !dbg !25
  %7 = shl i32 %6, 16, !dbg !25
  %8 = or i32 %7, %3, !dbg !25
  ret i32 %8, !dbg !25
}

define zeroext i16 @ntohs(i32 %v) nounwind readnone {
entry:
  tail call void @llvm.dbg.value(metadata !{i32 %v}, i64 0, metadata !19), !dbg !28
  %0 = trunc i32 %v to i16, !dbg !29
  tail call void @llvm.dbg.value(metadata !{i16 %0}, i64 0, metadata !17) nounwind, !dbg !31
  %1 = tail call i16 @llvm.bswap.i16(i16 %0) nounwind
  ret i16 %1, !dbg !29
}

define i32 @ntohl(i32 %v) nounwind readnone {
entry:
  tail call void @llvm.dbg.value(metadata !{i32 %v}, i64 0, metadata !20), !dbg !32
  tail call void @llvm.dbg.value(metadata !{i32 %v}, i64 0, metadata !18) nounwind, !dbg !33
  %0 = lshr i32 %v, 16, !dbg !36
  %1 = trunc i32 %0 to i16, !dbg !36
  tail call void @llvm.dbg.value(metadata !{i16 %1}, i64 0, metadata !17) nounwind, !dbg !37
  %2 = tail call i16 @llvm.bswap.i16(i16 %1) nounwind
  %3 = zext i16 %2 to i32, !dbg !36
  %4 = trunc i32 %v to i16, !dbg !36
  tail call void @llvm.dbg.value(metadata !{i16 %4}, i64 0, metadata !17) nounwind, !dbg !37
  %5 = tail call i16 @llvm.bswap.i16(i16 %4) nounwind
  %6 = zext i16 %5 to i32, !dbg !36
  %7 = shl i32 %6, 16, !dbg !36
  %8 = or i32 %7, %3, !dbg !36
  ret i32 %8, !dbg !34
}

!llvm.dbg.sp = !{!0, !8, !13, !16}
!llvm.dbg.lv.htons = !{!17}
!llvm.dbg.lv.htonl = !{!18}
!llvm.dbg.lv.ntohs = !{!19}
!llvm.dbg.lv.ntohl = !{!20}

!0 = metadata !{i32 524334, i32 0, metadata !1, metadata !"htons", metadata !"htons", metadata !"htons", metadata !1, i32 26, metadata !3, i1 false, i1 true, i32 0, i32 0, null, i1 false, i1 true, i16 (i16)* @htons} ; [ DW_TAG_subprogram ]
!1 = metadata !{i32 524329, metadata !"htonl.c", metadata !"/home/xuanji/klee-nush/runtime/klee-libc/", metadata !2} ; [ DW_TAG_file_type ]
!2 = metadata !{i32 524305, i32 0, i32 1, metadata !"htonl.c", metadata !"/home/xuanji/klee-nush/runtime/klee-libc/", metadata !"4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2.8)", i1 true, i1 true, metadata !"", i32 0} ; [ DW_TAG_compile_unit ]
!3 = metadata !{i32 524309, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !4, i32 0, null} ; [ DW_TAG_subroutine_type ]
!4 = metadata !{metadata !5, metadata !5}
!5 = metadata !{i32 524310, metadata !6, metadata !"uint16_t", metadata !6, i32 52, i64 0, i64 0, i64 0, i32 0, metadata !7} ; [ DW_TAG_typedef ]
!6 = metadata !{i32 524329, metadata !"stdint.h", metadata !"/usr/include", metadata !2} ; [ DW_TAG_file_type ]
!7 = metadata !{i32 524324, metadata !1, metadata !"short unsigned int", metadata !1, i32 0, i64 16, i64 16, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!8 = metadata !{i32 524334, i32 0, metadata !1, metadata !"htonl", metadata !"htonl", metadata !"htonl", metadata !1, i32 29, metadata !9, i1 false, i1 true, i32 0, i32 0, null, i1 false, i1 true, i32 (i32)* @htonl} ; [ DW_TAG_subprogram ]
!9 = metadata !{i32 524309, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !10, i32 0, null} ; [ DW_TAG_subroutine_type ]
!10 = metadata !{metadata !11, metadata !11}
!11 = metadata !{i32 524310, metadata !6, metadata !"uint32_t", metadata !6, i32 59, i64 0, i64 0, i64 0, i32 0, metadata !12} ; [ DW_TAG_typedef ]
!12 = metadata !{i32 524324, metadata !1, metadata !"unsigned int", metadata !1, i32 0, i64 32, i64 32, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!13 = metadata !{i32 524334, i32 0, metadata !1, metadata !"ntohs", metadata !"ntohs", metadata !"ntohs", metadata !1, i32 44, metadata !14, i1 false, i1 true, i32 0, i32 0, null, i1 false, i1 true, i16 (i32)* @ntohs} ; [ DW_TAG_subprogram ]
!14 = metadata !{i32 524309, metadata !1, metadata !"", metadata !1, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !15, i32 0, null} ; [ DW_TAG_subroutine_type ]
!15 = metadata !{metadata !5, metadata !11}
!16 = metadata !{i32 524334, i32 0, metadata !1, metadata !"ntohl", metadata !"ntohl", metadata !"ntohl", metadata !1, i32 47, metadata !9, i1 false, i1 true, i32 0, i32 0, null, i1 false, i1 true, i32 (i32)* @ntohl} ; [ DW_TAG_subprogram ]
!17 = metadata !{i32 524545, metadata !0, metadata !"v", metadata !1, i32 26, metadata !5} ; [ DW_TAG_arg_variable ]
!18 = metadata !{i32 524545, metadata !8, metadata !"v", metadata !1, i32 29, metadata !11} ; [ DW_TAG_arg_variable ]
!19 = metadata !{i32 524545, metadata !13, metadata !"v", metadata !1, i32 44, metadata !11} ; [ DW_TAG_arg_variable ]
!20 = metadata !{i32 524545, metadata !16, metadata !"v", metadata !1, i32 47, metadata !11} ; [ DW_TAG_arg_variable ]
!21 = metadata !{i32 26, i32 0, metadata !0, null}
!22 = metadata !{i32 27, i32 0, metadata !23, null}
!23 = metadata !{i32 524299, metadata !0, i32 26, i32 0, metadata !1, i32 0} ; [ DW_TAG_lexical_block ]
!24 = metadata !{i32 29, i32 0, metadata !8, null}
!25 = metadata !{i32 30, i32 0, metadata !26, null}
!26 = metadata !{i32 524299, metadata !8, i32 29, i32 0, metadata !1, i32 1} ; [ DW_TAG_lexical_block ]
!27 = metadata !{i32 26, i32 0, metadata !0, metadata !25}
!28 = metadata !{i32 44, i32 0, metadata !13, null}
!29 = metadata !{i32 45, i32 0, metadata !30, null}
!30 = metadata !{i32 524299, metadata !13, i32 44, i32 0, metadata !1, i32 2} ; [ DW_TAG_lexical_block ]
!31 = metadata !{i32 26, i32 0, metadata !0, metadata !29}
!32 = metadata !{i32 47, i32 0, metadata !16, null}
!33 = metadata !{i32 29, i32 0, metadata !8, metadata !34}
!34 = metadata !{i32 48, i32 0, metadata !35, null}
!35 = metadata !{i32 524299, metadata !16, i32 47, i32 0, metadata !1, i32 3} ; [ DW_TAG_lexical_block ]
!36 = metadata !{i32 30, i32 0, metadata !26, metadata !34}
!37 = metadata !{i32 26, i32 0, metadata !0, metadata !36}
