```dataview
table without id
file.link as "Name",
file.mday as "Last Modified", 
filter(file.tags, (t) => t != "#" + this.file.name) as "Other Tags",
file.outlinks as "Outgoing Links"
where contains(file.tags, "#" + this.file.name)
```
