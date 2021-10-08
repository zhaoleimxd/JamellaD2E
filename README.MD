English | [简体中文](README_zhCN.MD)
## Jamella's Diablo 2 Hero Editor ##
###### Full source code and  solution & project of **Visual Studio 2019**
---------------------------------------------------------------
When I was a child, I've downloaded JamellaD2E, and I used it to edit my characters and equipments, until now I found the full source code of JD2E, What a coincidence.

---------------------------------------------------------------
#### Note
The source code of JD2E is published by this PDF file: [jamella-diablo-ii-source-code.pdf](docs/jamella-diablo-ii-source-code.pdf)

I've dumped it into cpp source code files, and fixed it for compiling in VS2019.


This is the original read me from the PDF by Jamella:
>---------------------------------------------------------------
> #### <center>Source Code of Jamella’s Diablo 2 Editor</center>
> #### <center>2001-03-17</center>
>
>This source code is my intellectual property. It was completely written by myself. I have published it so fellow programmers and other interested people can have a look into the internals of my editor.
>
>If you use parts of the code consider that I needed over half a year to build this program and mention me in your info box if you like.
>
>Other than that your will find some modifications I made after releasing version 3.0 of the editor, so don’t be surprised if you find some extra functions and useless pieces of code.
>
>And please don’t email me and ask me how the code works, because it does and I don’t have time to teach the whole world object-orientated C++. Now dig into it by first reading the header file.
>
>
>
>Jamella
>
>http://jamella.cjb.net
>
>jamella@gmx.net
>
>---------------------------------------------------------------

#### Gathering source code:

```
1. Use <Google Chrome> to open the PDF File.
2. Ctrl + C/V copy all into <pdf.txt>.
3. Remove the first page(readme part) from txt.
4. Use RegEx to get both <File Name> and <File Content>.
5. Save to source file.
```

RegEx steps is in this file: [index.ts](typescript/index.ts)

It is written in TypeScript and run by NodeJS, and no longer need to run now.

Now the source code cannot be compiled yet, because there still has some problem to be solve.

---------------------------------------------------------------
#### Line break problem

First, there is a line break problem in most files, those lines has breaked at wrong position, like in the middle of strings, this problem comes out because PDF file has automatically wrapped at some lines which its width is too long, so I used VS2019 and VSCode to formatted and patched all files to make sure all code files doesn't show any error during just viewing in VS2019 IDE.

---------------------------------------------------------------
#### Gathering resource files

I've got a `JamellaD2E v3.0` executable file from [tytannial](https://github.com/tytannial) in [#1](https://github.com/zhaoleimxd/JamellaD2E/issues/1), and I extracted all the missing resource files from the exe, it works perfectly now.

---------------------------------------------------------------
#### Compile

##### Warnings
There are a lot of warnings while compiling, because originally this project is written in VC98, so I've disabled these warning belows.
```
26451;6328;28183;6011;6386;4996;4552;4477;4554;4474;4258;
```

##### Loop problem
There is an intresting thing in source code, Jamella write loops like this:
```
for (int i = 0; i < 100; i++>) {
    // code...
}

// some other code...
int a = i; // he was using variant 'i' without defination, because it has been defined in the loop above.
```
This habit will cause error in VS2019, so I patched all these codes which show an error while compiling.

##### CRT Warnings
Because this project is written at VC6 age, the C Runtime has a lot different with now, I need to add `_CRT_SECURE_NO_WARNINGS` macro to make sure CRT warnings no longer shows.

##### Macros
JamellaD2E seems has two projects, and you can see it in `JamellaD2E.h`
* Jamella's Diablo 2 Hero Editor
* Jamella's COM Object

And he use macros to control definitions in source code.

Macro|Project
---|:---:
`JAMELLAEDITOR`|Jamella's Diablo 2 Hero Editor
`JAMELLACOMSERVER`|Jamella's COM Object

I've added `JAMELLAEDITOR` macro for this project.

There are still a lot of macros in `JamellaD2E.h`, you can check it if you are intresting in it.

##### Const string problem
Back in VC6's age, you can write code like this without warnings and errors:
```
char* sz = "Hello, World.";
```
And now in VS2019, it will show you warnings and errors, you must write like this:
```
const char* sz = "Hello, World.";
```
To solve this problem without modifying source code, I changed `permissive mode` to false for this project.

And also I disabled `SDL check`.

---------------------------------------------------------------
#### Link
Now, we can compile all files, but missing some static import library files while linking.

* `comctl32.lib` for using Windows Common Controls.
* `Winmm.lib` for playing sounds.

I've added it into project configuration.

---------------------------------------------------------------
#### Done
After all these works, we got a executable file of JamellaD2E, this is just what I want, I can build & debug it, and see how it works.

---------------------------------------------------------------
* Written & Open source by `Jamella`
* Convert to VS2019 solution by `zhaoleimxd`
* Special thanks to `Nedkali` for resource files
* Special thanks to `tytannial` for provide a v3.0 exe to extract resource files

2021-10-09
