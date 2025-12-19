# DBView 


### Compiling and running

Pull the repo and run

```sh
make build
```

A binary `dbview` will be generate in `/bin` folder.


### Creating a new database

```sh
./bin/dbview -f <your-file> -n 
```

### Addind an employee

```sh
./bin/dbview -f <your-file> -a "Employee name, address, hours"
```

### Listing all the employees

```sh
./bin/dbview -f <your-file> -l
```

### Deleting an employee record

```sh
./bin/dbview -f <your-file> -d "Employee name"
```

### Updating an employee data

```sh
./bin/dbview -f <your-file> -d "Employee name, new employee name, new address,new hours"
````
