import utils
import re
from pathlib import Path

def cpp_has_header(cpp_file_name: str, header_files: [str]):
    for header_file in header_files:
        if f"{cpp_file_name}." in header_file:
            return True

    return False

def corresponding_header_is_first(cpp_file: str):
    file_contents = open(cpp_file, "r", encoding="utf-8").read()
    file_name = Path(cpp_file).stem
    first_include = re.search("#include.*\\n", file_contents, re.MULTILINE)
    header_regex = re.compile(f"#include.*{file_name}.(hpp|h)")
    return header_regex.search(first_include.group(0))

def verify_all_cpp_files_have_correct_first_header(all_cpp_files: [str], all_header_files: [str]):
    cpp_files_have_correct_first_include = True

    for cpp_file in all_cpp_files:
        file_name = Path(cpp_file).stem
        if cpp_has_header(file_name, all_header_files):
            if not corresponding_header_is_first(cpp_file):
                print(f"File {cpp_file} should include {file_name}.h or {file_name}.hpp as the first include")
                cpp_files_have_correct_first_include = False

    return cpp_files_have_correct_first_include

def verify_no_relative_includes(all_cpp_files: [str], all_header_files: [str]):
    all_files_have_no_relative_includes = True
    relative_include_regex = re.compile(f"#include.*\\.\\/.*\\.h")

    for file in all_cpp_files + all_header_files:
        with open(file, "r", encoding="utf-8") as f:
            file_contents = f.read()
            if relative_include_regex.search(file_contents):
                print(f"File {file} has relative includes, which is not allowed")
                all_files_have_no_relative_includes = False

    return all_files_have_no_relative_includes

def main():
    all_header_files = utils.find_all_files_with_extension([".hpp", ".h"])
    all_cpp_files = utils.find_all_files_with_extension([".c", ".cc", ".cxx", ".cpp"])

    cpp_files_have_correct_first_include = verify_all_cpp_files_have_correct_first_header(all_cpp_files,
                                                                                          all_header_files)

    files_have_no_relative_includes = verify_no_relative_includes(all_cpp_files, all_header_files)

    if not cpp_files_have_correct_first_include or not files_have_no_relative_includes:
        exit(1)
    exit(0)


if __name__ == "__main__":
    main()