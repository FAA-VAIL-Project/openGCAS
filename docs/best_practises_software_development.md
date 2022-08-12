# Software Development Best Practises

Here we define the standards and best practices we follow when developing software.
In addition, programming language specific guidelines are available in the following documents:

- [C++ Best Practises](best_practises_cpp.md)

## Documentation

- the documentation in the repository is usually created using GitHub variant of Markdown - [see here](https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax){:target="_blank"}
- the top directory of a repository generally contains the following documentation files:
  - **CODE_OF_CONDUCT.md** - defines standards for how to engage in the project
  - **CONTRIBUTING.md** - guidelines to communicate how people should contribute to the project
  - **LICENSE** - the legal instrument governing the use or redistribution of the software
  - **README.md** - typically includes information on:
    - What the project does
    - Why the project is useful
    - How users can get started with the project
    - Where users can get help with your project
    - Who maintains and contributes to the project
- all subdirectories must contain a file called **README**.md, which briefly describes what is contained in this directory
- the rest of the project documentation files are located in the `docs` subdirectory
- the project documentation is created with the tool [MkDocs](https://www.mkdocs.org)
