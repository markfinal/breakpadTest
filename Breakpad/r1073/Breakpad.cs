// Automatically generated by Opus v0.50
namespace Breakpad
{
    class BreakpadClientStaticLibrary : C.StaticLibrary
    {
        public BreakpadClientStaticLibrary()
        {
            this.headers.Include(this, "src", "src", "common", "windows", "*.h");
            this.headers.Include(this, "src", "src", "client", "windows", "crash_generation", "*.h");
            this.headers.Include(this, "src", "src", "client", "windows", "handler", "*.h");
        }

        class SourceFiles : C.CPlusPlus.ObjectFileCollection
        {
            public SourceFiles()
            {
                this.Include(this, "src", "src", "common", "windows", "guid_string.cc");
                this.Include(this, "src", "src", "client", "windows", "crash_generation", "crash_generation_client.cc");
                this.Include(this, "src", "src", "client", "windows", "handler", "exception_handler.cc");

                this.UpdateOptions += new Opus.Core.UpdateOptionCollectionDelegate(SourceFiles_UpdateOptions);
                this.UpdateOptions += new Opus.Core.UpdateOptionCollectionDelegate(SourceFiles_UpdateOptions2);
            }

            void SourceFiles_UpdateOptions2(Opus.Core.IModule module, Opus.Core.Target target)
            {
                C.ICCompilerOptions compilerOptions = module.Options as C.ICCompilerOptions;
                compilerOptions.WarningsAsErrors = false;
                compilerOptions.IncludePaths.Include(this, "src", "src");

                // for MSVC2008
                compilerOptions.Defines.Add("STATUS_INVALID_PARAMETER=0xC000000DL");

                C.ICPlusPlusCompilerOptions cxxCompilerOptions = module.Options as C.ICPlusPlusCompilerOptions;
                cxxCompilerOptions.ExceptionHandler = C.CPlusPlus.EExceptionHandler.Asynchronous;

                C.IToolchainOptions toolchainOptions = compilerOptions.ToolchainOptionCollection as C.IToolchainOptions;
                toolchainOptions.CharacterSet = C.ECharacterSet.Unicode;
            }

            [C.ExportCompilerOptionsDelegate]
            void SourceFiles_UpdateOptions(Opus.Core.IModule module, Opus.Core.Target target)
            {
                C.ICCompilerOptions compilerOptions = module.Options as C.ICCompilerOptions;
                compilerOptions.IncludePaths.Include(this, "src", "src");
            }
        }

        [Opus.Core.SourceFiles]
        SourceFiles source = new SourceFiles();

        [C.HeaderFiles]
        Opus.Core.FileCollection headers = new Opus.Core.FileCollection();

        [Opus.Core.DependentModules]
        Opus.Core.TypeArray dependents = new Opus.Core.TypeArray(
            typeof(WindowsSDK.WindowsSDK)
        );
    }

    class BreakpadServerStaticLibrary : C.StaticLibrary
    {
        public BreakpadServerStaticLibrary()
        {
            this.headers.Include(this, "src", "src", "common", "windows", "*.h");
            this.headers.Include(this, "src", "src", "client", "windows", "crash_generation", "*.h");
        }

        class SourceFiles : C.CPlusPlus.ObjectFileCollection
        {
            public SourceFiles()
            {
                this.Include(this, "src", "src", "common", "windows", "guid_string.cc");
                this.Include(this, "src", "src", "client", "windows", "crash_generation", "crash_generation_server.cc");
                this.Include(this, "src", "src", "client", "windows", "crash_generation", "minidump_generator.cc");
                this.Include(this, "src", "src", "client", "windows", "crash_generation", "client_info.cc");

                this.UpdateOptions += new Opus.Core.UpdateOptionCollectionDelegate(SourceFiles_UpdateOptions);
                this.UpdateOptions += new Opus.Core.UpdateOptionCollectionDelegate(SourceFiles_UpdateOptions2);
            }

            void SourceFiles_UpdateOptions2(Opus.Core.IModule module, Opus.Core.Target target)
            {
                C.ICCompilerOptions compilerOptions = module.Options as C.ICCompilerOptions;

                C.ICPlusPlusCompilerOptions cxxCompilerOptions = module.Options as C.ICPlusPlusCompilerOptions;
                cxxCompilerOptions.ExceptionHandler = C.CPlusPlus.EExceptionHandler.Asynchronous;

                C.IToolchainOptions toolchainOptions = compilerOptions.ToolchainOptionCollection as C.IToolchainOptions;
                toolchainOptions.CharacterSet = C.ECharacterSet.Unicode;
            }

            [C.ExportCompilerOptionsDelegate]
            void SourceFiles_UpdateOptions(Opus.Core.IModule module, Opus.Core.Target target)
            {
                C.ICCompilerOptions compilerOptions = module.Options as C.ICCompilerOptions;
                compilerOptions.IncludePaths.Include(this, "src", "src");
            }
        }

        [Opus.Core.SourceFiles]
        SourceFiles source = new SourceFiles();

        [C.HeaderFiles]
        Opus.Core.FileCollection headers = new Opus.Core.FileCollection();

        [Opus.Core.DependentModules]
        Opus.Core.TypeArray dependents = new Opus.Core.TypeArray(
            typeof(WindowsSDK.WindowsSDK)
        );
    }
}
