*What is something you would like to see added to C++?*

This is a pretty silly request, but oftenhand when I write more complex programs I like to have configuration files. When using Python, I can usually simply use the built-in `json` library, or `pyYAML` if I feel fancy. However in C++, there's no simple tool that allows me to read and write to these hierarchical types of files. It would be nice to have a header that gives developers the ability to read these files and store their contents in some map. Obviously this object would be statically-typed:

```c++
class JSON{
    std::hashmap<auto, std::any> data;
    std::string filename;
    Permission permission;
public:
    JSON(std::string filename, Permission permission): filename(filename), permission(permission) {}
    std::hashmap<auto, std::any> read(); //some recursive function that would return the contents of the file
    void write(std::hashmap<auto, std::any>); //some function that would write the file according to a template
    // etc.
};
```

I like the fact that C++ allows for a lot of flexibility for I/O operations with files, but for this kind of task, often writing the program that interprets configuration files, feels like reinventing the wheel. 

Additionnaly a library like **CLI11** should be in the standard template library. For the same reasons as the configuration file problem stated above.
