#include <SDCardModule.h>

#include <pico/stdlib.h>
#include <stdio.h>


int main()
{
    stdio_init_all();
    
    printf("Program Start\n");

    uazips::SDCardModule::PinSetup pins = {2,3,4,5};

    uazips::SDCardModule mod("A", pins);

    uazips::Module::InitAll();

    mod.Mount();

    mod.OpenFile("HelloWorld.txt");

    mod.WriteString("Hello");

    mod.Close();

    printf("Program End\n");

    return 0;
}