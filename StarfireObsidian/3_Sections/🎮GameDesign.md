```dataview
table without id 

join(slice(split(file.folder, "/"), 1, -1), "-") + "**" + split(file.folder, "/")[length(split(file.folder, "/")) - 1] + " **- " + file.link as "Name",

file.mday as "Last Modified", 

filter(file.tags, (t) => t != "#" + this.file.name) as "Other Tags"


where contains(file.tags, "#" + this.file.name)
```
