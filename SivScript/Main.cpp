﻿
#include <Siv3D.hpp> // OpenSiv3D v0.3.0
#include <filesystem>

namespace {
    // Windows.hを読み込む前に呼ばないとSetCurrentDirectoryが
    // マクロで上書きされてしまいうまく動かない。
	// test
    void SetDir(const FilePath& path)
    {
        ::s3d::Windows::FileSystem::SetCurrentDirectory(path);
    }
}

#include <Windows.h>

namespace {
    Array<String> getArgs()
    {
        auto argv = Array<String>();

        int nArgs = 0;
        LPWSTR* szArglist = ::CommandLineToArgvW(::GetCommandLineW(), &nArgs);

        for (int i = 1; i < nArgs; i++) {
            argv << Unicode::FromWString(szArglist[i]);
        }
        ::LocalFree(szArglist);

        return argv;
    }
}

void Main()
{
    SetDir(::s3d::Windows::FileSystem::WorkingDirectory());

    auto args = getArgs();

    auto path = std::filesystem::path(args[0].toUTF32());
    SetDir(path.parent_path().u32string());

    ManagedScript main(path.filename().u32string());

    while (System::Update())
    {
        main.run();
    }
}
