// RUN: %clang_cc1 -fms-extensions -triple i686-windows-msvc   -emit-llvm %s -o - | FileCheck --check-prefix=I386 %s
// RUN: %clang_cc1 -fms-extensions -triple x86_64-windows-msvc -emit-llvm %s -o - | FileCheck --check-prefix=X64 %s

typedef char jmp_buf[1];

int _setjmp(jmp_buf env);
int _setjmpex(jmp_buf env);

jmp_buf jb;

int test_setjmp() {
  return _setjmp(jb);
  // I386-LABEL: define i32 @test_setjmp
  // I386:       %[[call:.*]] = call i32 (i8*, i32, ...)* @_setjmp3(i8* getelementptr inbounds ([1 x i8]* @jb, i32 0, i32 0), i32 0)
  // I386-NEXT:  ret i32 %[[call]]

  // X64-LABEL: define i32 @test_setjmp
  // X64:       %[[addr:.*]] = call i8* @llvm.frameaddress(i32 0)
  // X64:       %[[call:.*]] = call i32 @_setjmp(i8* getelementptr inbounds ([1 x i8]* @jb, i32 0, i32 0), i8* %[[addr]])
  // X64-NEXT:  ret i32 %[[call]]
}

int test_setjmpex() {
  return _setjmpex(jb);
  // X64-LABEL: define i32 @test_setjmpex
  // X64:       %[[addr:.*]] = call i8* @llvm.frameaddress(i32 0)
  // X64:       %[[call:.*]] = call i32 @_setjmpex(i8* getelementptr inbounds ([1 x i8]* @jb, i32 0, i32 0), i8* %[[addr]])
  // X64-NEXT:  ret i32 %[[call]]
}
