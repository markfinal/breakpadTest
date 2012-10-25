// Automatically generated by Opus v0.50
namespace Test
{
    class CrashServer : C.Application
    {
        public CrashServer()
        {
            this.UpdateOptions += new Opus.Core.UpdateOptionCollectionDelegate(CrashServer_UpdateOptions);
        }

        void CrashServer_UpdateOptions(Opus.Core.IModule module, Opus.Core.Target target)
        {
            C.ILinkerOptions linkerOptions = module.Options as C.ILinkerOptions;
            linkerOptions.DoNotAutoIncludeStandardLibraries = false;
        }

        class Source : C.CPlusPlus.ObjectFileCollection
        {
            public Source()
            {
                this.Include(this, "src", "crashservermain_win.cpp");

                this.UpdateOptions += new Opus.Core.UpdateOptionCollectionDelegate(Source_UpdateOptions);
            }

            void Source_UpdateOptions(Opus.Core.IModule module, Opus.Core.Target target)
            {
                C.ICCompilerOptions compilerOptions = module.Options as C.ICCompilerOptions;
                compilerOptions.IgnoreStandardIncludePaths = false;

                C.ICPlusPlusCompilerOptions cxxCompilerOptions = module.Options as C.ICPlusPlusCompilerOptions;
                cxxCompilerOptions.ExceptionHandler = C.CPlusPlus.EExceptionHandler.Asynchronous;
            }
        }

        [Opus.Core.SourceFiles]
        Source source = new Source();

        [Opus.Core.DependentModules]
        Opus.Core.TypeArray dependents = new Opus.Core.TypeArray(
            typeof(Breakpad.BreakpadServerStaticLibrary),
            typeof(WindowsSDK.WindowsSDK)
            );
    }

    class ConsoleTest : C.Application
    {
        public ConsoleTest()
        {
            this.headers.Include(this, "src", "*.h");

            this.UpdateOptions += new Opus.Core.UpdateOptionCollectionDelegate(ConsoleTest_UpdateOptions);
        }

        void ConsoleTest_UpdateOptions(Opus.Core.IModule module, Opus.Core.Target target)
        {
            C.ILinkerOptions linkerOptions = module.Options as C.ILinkerOptions;
            linkerOptions.DoNotAutoIncludeStandardLibraries = false;
        }

        class Source : C.CPlusPlus.ObjectFileCollection
        {
            public Source()
            {
                this.Include(this, "src", "main.cpp");
                this.Include(this, "src", "crashhandler_win.cpp");
                this.UpdateOptions += new Opus.Core.UpdateOptionCollectionDelegate(Source_UpdateOptions);
            }

            void Source_UpdateOptions(Opus.Core.IModule module, Opus.Core.Target target)
            {
                C.ICCompilerOptions compilerOptions = module.Options as C.ICCompilerOptions;
                compilerOptions.IgnoreStandardIncludePaths = false;

                C.ICPlusPlusCompilerOptions cxxCompilerOptions = module.Options as C.ICPlusPlusCompilerOptions;
                cxxCompilerOptions.ExceptionHandler = C.CPlusPlus.EExceptionHandler.Asynchronous;
            }
        }

        [Opus.Core.SourceFiles]
        Source source = new Source();

        [C.HeaderFiles]
        Opus.Core.FileCollection headers = new Opus.Core.FileCollection();

        [Opus.Core.DependentModules]
        Opus.Core.TypeArray dependencies = new Opus.Core.TypeArray(
            typeof(WindowsSDK.WindowsSDK),
            typeof(Breakpad.BreakpadClientStaticLibrary)
        );
    }
}
