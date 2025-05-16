# CMakeのバージョンを設定
cmake_minimum_required(VERSION 3.13)

# プロジェクト名と使用する言語を設定
project(miniArchive CXX)
# miniArchiveという実行ファイルをarchive.cppとarchive.cppから作成
add_executable(miniArchive sample.cpp archive.cpp)