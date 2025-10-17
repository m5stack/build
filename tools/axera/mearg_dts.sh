#!/bin/bash

input_file="$1"
output_file="$2"
patch_file=`basename $input_file`
file_header="--- a/arch/arm64/boot/dts/axera/${patch_file}	2025-10-15 11:29:39.695430889 +0800
+++ b/arch/arm64/boot/dts/axera/${patch_file}	2025-10-15 11:33:17.587739273 +0800
@@ -0,0 +1,1048 @@"
line_count=1048

# 写入文件头
echo "$file_header" > "$output_file"

# 使用 awk 读文件，加 +号，满1034行后截断，不足时补空行
awk -v n="$line_count" '
    NR <= n         { print "+" $0 }
    END {
        for(i=NR+1;i<=n;i++) print "+"
    }
' "$input_file" >> "$output_file"