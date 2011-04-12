
@rem DLL.
xcopy "Weibo.common/bin/DLL Debug" "Weibo.sdk/third-party/bin/DLL Debug" /E /Y /C /I
xcopy "Weibo.common/bin/DLL Release" "Weibo.sdk/third-party/bin/DLL Release" /E /Y /C /I

@rem Lib file.
xcopy "Weibo.common/lib/DLL Debug" "Weibo.sdk/third-party/lib/DLL Debug" /E /Y /C /I
xcopy "Weibo.common/lib/DLL Release" "Weibo.sdk/third-party/lib/DLL Release" /E /Y /C /I

@rem Include file,if you need,please turn it on.
::xcopy "Weibo.common/include" "Weibo.sdk/third-party/include" /E /Y /C /I


pause
