import os
import re

copyright_regex = re.compile(r"\/\*\n\s\*\sCopyright\s.\s(\d{4}-*\d{4}|\d*)\sTim\sHerreijgers\n\s\*\sLicensed\susing\sthe\sMIT\slicense\n\s\*\/")

def is_in_excluded_folder(root: str):
    ignored_folders = ["build", "third_party", ".git", ".idea", "scripts", ".github", "cmake"]

    for folder in ignored_folders:
        if folder in root:
            return True

    return False

def has_any_of_file_extension(file: str, extensions):

    for file_extension in extensions:
        if file.endswith(file_extension):
            return True

    return False

def find_all_files_with_extension(extensions):
    cpp_files = []

    for root, dirs, files in os.walk('.'):
        if is_in_excluded_folder(root):
            continue

        for file in files:
            if has_any_of_file_extension(file, extensions):
                cpp_files.append(os.path.join(root, file))

    return cpp_files