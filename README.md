# V8Study.x86
MAC下编译和运行 V8的例子

### V8编译脚本 args.gn
```
is_debug = false
target_cpu = "x64"
use_custom_libcxx=false
is_component_build = false
is_clang = false
use_lld = false
v8_static_library = true
v8_monolithic = true
v8_use_external_startup_data = false
v8_enable_testtrue_features = false
v8_enable_i18n_support = false
treat_warnings_as_errors = false
symbol_level = 1
```