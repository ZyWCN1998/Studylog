# 20230812 Learn C the Hardway 17 04

按教材输入程序，运行，并查看结果

```c
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
  int id;
  int set;
  char name[MAX_DATA];
  char email[MAX_DATA];
};

struct Database {
  struct Address rows[MAX_ROWS];
};

struct Connection {
  FILE *file;
  struct Database *db;
};

void die(const char *message) {
  if (errno) {
    perror(message);
  } else {
    printf("ERROR: %s\n", message);
  }
  exit(1);
}

void Address_print(struct Address *addr) {
  printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn) {
  int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
  if (rc != 1)
    die("Failed to load database");
}

struct Connection *Database_open(const char *filename, char mode) {
  struct Connection *conn = malloc(sizeof(struct Connection));
  if (!conn)
    die("Memory error");

  conn->db = malloc(sizeof(struct Database));
  if (!conn->db)
    die("Memory error");

  if (mode == 'c') {
    conn->file = fopen(filename, "w");

  } else {
    conn->file = fopen(filename, "r+");

    if (conn->file) {
      Database_load(conn);
    }
  }
  if (!conn->file)
    die("Failed to open the file");
  return conn;
}

void Database_close(struct Connection *conn) {
  if (conn) {
    if (conn->file)
      fclose(conn->file);
    if (conn->db)
      free(conn->db);
    free(conn);
  }
}

void Database_write(struct Connection *conn) {
  rewind(conn->file);

  int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
  if (rc != 1)
    die("Failed to write database.");

  rc = fflush(conn->file);
  if (rc == -1)
    die("Cannot flush database");
}

void Database_create(struct Connection *conn) {
  int i = 0;

  for (i = 0; i < MAX_ROWS; i++) {
    // make a prototype to initialize it
    struct Address addr = {.id = i, .set = 0};
    // then just assign it
    conn->db->rows[i] = addr;
  }
}

void Database_set(struct Connection *conn, int id, const char *name,
                  const char *email) {
  struct Address *addr = &conn->db->rows[id];
  if (addr->set)
    die("Already set, delete it first");

  addr->set = 1;
  // WARNING:bug, read the "How To Break It" and fix this
  char *res = strncpy(addr->name, name, MAX_DATA);
  // demonstrate the strncpy bug
  if (!res)
    die("Name copy failed");

  res = strncpy(addr->email, email, MAX_DATA);
  if (!res)
    die("Email copy failed");
}

void Database_get(struct Connection *conn, int id) {
  struct Address *addr = &conn->db->rows[id];

  if (addr->set) {
    Address_print(addr);
  } else {
    die("ID is not set");
  }
}

void Database_delete(struct Connection *conn, int id) {
  struct Address addr = {.id = id, .set = 0};
  conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn) {
  int i = 0;
  struct Database *db = conn->db;

  for (i = 0; i < MAX_ROWS; i++) {
    struct Address *cur = &db->rows[i];

    if (cur->set) {
      Address_print(cur);
    }
  }
}

int main(int argc, char *argv[]) {

  if (argc < 3)
    die("USAGE: ex17 <dbfile><action> [action params]");
  char *filename = argv[1];
  char action = argv[2][0];
  struct Connection *conn = Database_open(filename, action);
  int id = 0;

  if (argc > 3)
    id = atoi(argv[3]);
  if (id >= MAX_ROWS)
    die("There is not many records. ");

  switch (action) {
  case 'c':
    Database_create(conn);
    Database_write(conn);
    break;
  case 'g':
    if (argc != 4)
      die("Need an id to get");

    Database_get(conn, id);
    break;
  case 's':
    if (argc != 6)
      die("Need id, name, email to set. ");

    Database_set(conn, id, argv[4], argv[5]);
    Database_write(conn);
    break;
  case 'd':
    if (argc != 4)
      die("Need id to delete. ");

    Database_delete(conn, id);
    Database_write(conn);
    break;
  case 'l':
    Database_list(conn);
    break;
  default:
    die("Invaild action: c=create, g=get, s=set, d=del, l=list");
  }
  Database_close(conn);

  return 0;
}
```

实现了一个简单的数据库

```bash
❯ make
cc -Wall -g    lecture17.c   -o lecture17

❯ ./lecture17 db.dat c

❯ ./lecture17 db.dat s 1 zed zed@shaw.com

❯ ./lecture17 db.dat s 2 frank frank@zedshaw.com

❯ ./lecture17 db.dat s 3 joe joe@zedshaw.com

❯ ./lecture17 db.dat 1
ERROR: Invaild action: c=create, g=get, s=set, d=del, l=list

❯ ./lecture17 db.dat l
1 zed zed@shaw.com
2 frank frank@zedshaw.com
3 joe joe@zedshaw.com

❯ ./lecture17 db.dat d3
ERROR: Need id to delete.

❯ ./lecture17 db.dat d 3

❯ ./lecture17 db.dat l
1 zed zed@shaw.com
2 frank frank@zedshaw.com

❯ ./lecture17 db.dat g 2
2 frank frank@zedshaw.com
```

# 破坏程序

你可以在很多地方破坏这个程序，试试下面这些，也自己想一些出来。

- 经典的方法是删除一些安全检查，这样你就可以传入任意数据。例如，删除第171行防止输入任意记录数据的检查
    
    ```c
    #include <assert.h>
    #include <errno.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    
    #define MAX_DATA 512
    #define MAX_ROWS 100
    
    struct Address {
      int id;
      int set;
      char name[MAX_DATA];
      char email[MAX_DATA];
    };
    
    struct Database {
      struct Address rows[MAX_ROWS];
    };
    
    struct Connection {
      FILE *file;
      struct Database *db;
    };
    
    void die(const char *message) {
      if (errno) {
        perror(message);
      } else {
        printf("ERROR: %s\n", message);
      }
      exit(1);
    }
    
    void Address_print(struct Address *addr) {
      printf("%d %s %s\n", addr->id, addr->name, addr->email);
    }
    
    void Database_load(struct Connection *conn) {
      int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
      if (rc != 1)
        die("Failed to load database");
    }
    
    struct Connection *Database_open(const char *filename, char mode) {
      struct Connection *conn = malloc(sizeof(struct Connection));
      if (!conn)
        die("Memory error");
    
      conn->db = malloc(sizeof(struct Database));
      if (!conn->db)
        die("Memory error");
    
      if (mode == 'c') {
        conn->file = fopen(filename, "w");
    
      } else {
        conn->file = fopen(filename, "r+");
    
        if (conn->file) {
          Database_load(conn);
        }
      }
      if (!conn->file)
        die("Failed to open the file");
      return conn;
    }
    
    void Database_close(struct Connection *conn) {
      if (conn) {
        if (conn->file)
          fclose(conn->file);
        if (conn->db)
          free(conn->db);
        free(conn);
      }
    }
    
    void Database_write(struct Connection *conn) {
      rewind(conn->file);
    
      int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
      if (rc != 1)
        die("Failed to write database.");
    
      rc = fflush(conn->file);
      if (rc == -1)
        die("Cannot flush database");
    }
    
    void Database_create(struct Connection *conn) {
      int i = 0;
    
      for (i = 0; i < MAX_ROWS; i++) {
        // make a prototype to initialize it
        struct Address addr = {.id = i, .set = 0};
        // then just assign it
        conn->db->rows[i] = addr;
      }
    }
    
    void Database_set(struct Connection *conn, int id, const char *name,
                      const char *email) {
      struct Address *addr = &conn->db->rows[id];
      if (addr->set)
        die("Already set, delete it first");
    
      addr->set = 1;
      // WARNING:bug, read the "How To Break It" and fix this
      char *res = strncpy(addr->name, name, MAX_DATA);
      // demonstrate the strncpy bug
      if (!res)
        die("Name copy failed");
    
      res = strncpy(addr->email, email, MAX_DATA);
      if (!res)
        die("Email copy failed");
    }
    
    void Database_get(struct Connection *conn, int id) {
      struct Address *addr = &conn->db->rows[id];
    
      if (addr->set) {
        Address_print(addr);
      } //else {
        //die("ID is not set");
      //}
    }
    
    void Database_delete(struct Connection *conn, int id) {
      struct Address addr = {.id = id, .set = 0};
      conn->db->rows[id] = addr;
    }
    
    void Database_list(struct Connection *conn) {
      int i = 0;
      struct Database *db = conn->db;
    
      for (i = 0; i < MAX_ROWS; i++) {
        struct Address *cur = &db->rows[i];
    
        if (cur->set) {
          Address_print(cur);
        }
      }
    }
    
    int main(int argc, char *argv[]) {
    
      if (argc < 3)
        die("USAGE: ex17 <dbfile><action> [action params]");
      char *filename = argv[1];
      char action = argv[2][0];
      struct Connection *conn = Database_open(filename, action);
      int id = 0;
    
      if (argc > 3)
        id = atoi(argv[3]);
      // if (id >= MAX_ROWS)
      //   die("There is not many records. ");
    
      switch (action) {
      case 'c':
        Database_create(conn);
        Database_write(conn);
        break;
      case 'g':
        if (argc != 4)
          die("Need an id to get");
    
        Database_get(conn, id);
        break;
      case 's':
        if (argc != 6)
          die("Need id, name, email to set. ");
    
        Database_set(conn, id, argv[4], argv[5]);
        Database_write(conn);
        break;
      case 'd':
        if (argc != 4)
          die("Need id to delete. ");
    
        Database_delete(conn, id);
        Database_write(conn);
        break;
      case 'l':
        Database_list(conn);
        break;
      default:
        die("Invaild action: c=create, g=get, s=set, d=del, l=list");
      }
      Database_close(conn);
    
      return 0;
    }
    ```
    
    ```bash
    #修改前
    ❯ ./lecture17 db.dat l
    1 zed zed@shaw.com
    2 frank frank@zedshaw.com
    ❯ ./lecture17 db.dat g 3
    ERROR: ID is not set
    
    #修改后
    ❯ ./lecture17 db.dat g 1
    1 zed zed@shaw.com
    
    ❯ ./lecture17 db.dat g 2
    2 frank frank@zedshaw.com
    
    ❯ ./lecture17 db.dat g 3
    ```
    
- 你还可以试着破坏数据文件。用编辑器打开文件，随机修改其中的字节。然后关闭文件。
    
    ```bash
    ❯ ./ex17 db.dat c
    ❯ ./ex17 db.dat s 1 zed zed@zedshaw.com
    ❯ ./ex17 db.dat g 1
    1 zed zed@zedshaw.com
    ```
    
    破坏之后
    
    ```bash
    ❯ ./lecture17 db.dat g 1
    ERROR: Failed to load database.
    ```