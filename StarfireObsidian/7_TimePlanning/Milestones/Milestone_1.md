```dataview
table filter(file.tags, (t) => !contains(t, "#" + this.file.name)) as "Other Tags", 
file.mday as "Last Modified" where contains(file.tags, "#" + this.file.name)
```