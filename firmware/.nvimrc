let g:ale_linters = { 'cpp': ['clangd'] }
let g:ale_fixers = { 'cpp': [ 'clang-format' ] }

let g:ale_c_clang_options="-I.pio/libdeps/esp32dev -Ilib"
let g:ale_cpp_clang_options="-I.pio/libdeps/esp32dev -Ilib"
let g:ale_c_clangd_options="-I.pio/libdeps/esp32dev -Ilib"
let g:ale_cpp_clangd_options="-I.pio/libdeps/esp32dev -Ilib"
