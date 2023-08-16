# 20230816 Learn C the Hardway 17 06

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
    
- 你还可以找到程序运行时传入错误参数的方法。例如：把文件名和动作反过来，它就会创建一个以动作命名的文件。然后根据第一个字符来执行这个动作。
    
    ```bash
    ❯ ./lecture17 c db.dat
    Failed to open the file: No such file or directory
    ```
    
- 因为strcpy设计很糟，所以这个程序中有一个缺陷。去查阅一下strcpy，试着找到当name或address大于512字节时会发生什么。将最后一个字符强制设为\0可以解决此问题，这样它就会永远设对，这也是strcpy该做的事情。
    
    ```bash
    NOTES
           Some  programmers  consider  strncpy()  to be inefficient and error prone.  If the programmer knows (i.e., in‐
           cludes code to test!)  that the size of dest is greater than the length of src, then strcpy() can be used.
    
           One valid (and intended) use of strncpy() is to copy a C string to a fixed-length buffer while  ensuring  both
           that  the  buffer is not overflowed and that unused bytes in the destination buffer are zeroed out (perhaps to
           prevent information leaks if the buffer is to be written to media or transmitted to another process via an in‐
           terprocess communication technique).
    
           If there is no terminating null byte in the first n bytes of src, strncpy() produces an unterminated string in
           dest.  If buf has length buflen, you can force termination using something like the following:
    
               if (buflen > 0) {
                   strncpy(buf, str, buflen - 1);
                   buf[buflen - 1]= '\0';
               }
    
           (Of course, the above technique ignores the fact that, if src contains more than buflen - 1 bytes, information
           is lost in the copying to dest.)
    
    BUGS
           If  the  destination string of a strcpy() is not large enough, then anything might happen.  Overflowing fixed-
           length string buffers is a favorite cracker technique for taking complete control of the machine.  Any time  a
           program  reads or copies data into a buffer, the program first needs to check that there's enough space.  This
           may be unnecessary if you can show that overflow is impossible, but be careful: programs can get changed  over
           time, in ways that may make the impossible possible.
    ```
    
    修改strcpy部分代码，按照manual的说明加入\0作为终止
    
    ```c
    void Database_set(struct Connection *conn, int id, const char *name,
                      const char *email) {
      struct Address *addr = &conn->db->rows[id];
      if (addr->set)
        die("Already set, delete it first");
    
      addr->set = 1;
      // WARNING:bug, read the "How To Break It" and fix this
      char *res = strncpy(addr->name, name, MAX_DATA);
      /*-----fix bug-----*/
      addr->name[MAX_DATA - 1] = '\0';
      /*-----fix bug-----*/
      // demonstrate the strncpy bug
      if (!res)
        die("Name copy failed");
    
      res = strncpy(addr->email, email, MAX_DATA);
      /*-----fix bug-----*/
      addr->email[MAX_DATA - 1] = '\0';
      /*-----fix bug-----*/
      if (!res)
        die("Email copy failed");
    }
    ```
    
- 在附加任务部分，我要求你加强程序，创建任意大小的数据库。试试看最大能创建多大的数据库，直到malloc无法获取足够内存而导致程序崩溃。
    
    这和物理机内存有关，我的虚拟机分了32G的内存，而且我并不是很想塞满我的内存
    

# 附加任务

- 你需要加强die函数，让它能传入conn变量，这样die就可以关闭连接并清理程序。
    
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
    
    void Database_close(struct Connection *conn);
    void die(const char *message, struct Connection *conn) {
      if (errno) {
        perror(message);
      } else {
        printf("ERROR: %s\n", message);
      }
    
      Database_close(conn);
      exit(1);
    }
    
    void Address_print(struct Address *addr) {
      printf("%d %s %s\n", addr->id, addr->name, addr->email);
    }
    
    void Database_load(struct Connection *conn) {
      int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
      if (rc != 1)
        die("Failed to load database", conn);
    }
    
    struct Connection *Database_open(const char *filename, char mode) {
      struct Connection *conn = malloc(sizeof(struct Connection));
      if (!conn)
        die("Memory error", conn);
    
      conn->db = malloc(sizeof(struct Database));
      if (!conn->db)
        die("Memory error", conn);
    
      if (mode == 'c') {
        conn->file = fopen(filename, "w");
    
      } else {
        conn->file = fopen(filename, "r+");
    
        if (conn->file) {
          Database_load(conn);
        }
      }
      if (!conn->file)
        die("Failed to open the file", conn);
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
        die("Failed to write database.", conn);
    
      rc = fflush(conn->file);
      if (rc == -1)
        die("Cannot flush database", conn);
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
        die("Already set, delete it first", conn);
    
      addr->set = 1;
      // WARNING:bug, read the "How To Break It" and fix this
      char *res = strncpy(addr->name, name, MAX_DATA);
      /*-----fix bug-----*/
      addr->name[MAX_DATA - 1] = '\0';
      /*-----fix bug-----*/
      // demonstrate the strncpy bug
      if (!res)
        die("Name copy failed", conn);
    
      res = strncpy(addr->email, email, MAX_DATA);
      /*-----fix bug-----*/
      addr->email[MAX_DATA - 1] = '\0';
      /*-----fix bug-----*/
      if (!res)
        die("Email copy failed", conn);
    }
    
    void Database_get(struct Connection *conn, int id) {
      struct Address *addr = &conn->db->rows[id];
    
      if (addr->set) {
        Address_print(addr);
      } else {
        die("ID is not set", conn);
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
        printf("USAGE: ex17 <dbfile><action> [action params]");
    		//die("USAGE: ex17 <dbfile> <action> [action params]", NULL);
      char *filename = argv[1];
      char action = argv[2][0];
      struct Connection *conn = Database_open(filename, action);
      int id = 0;
    
      if (argc > 3)
        id = atoi(argv[3]);
      if (id >= MAX_ROWS)
        printf("There is not many records. ");
    		//die("There's not that many records", NULL);
    
      switch (action) {
      case 'c':
        Database_create(conn);
        Database_write(conn);
        break;
      case 'g':
        if (argc != 4)
          die("Need an id to get", conn);
    
        Database_get(conn, id);
        break;
      case 's':
        if (argc != 6)
          die("Need id, name, email to set. ", conn);
    
        Database_set(conn, id, argv[4], argv[5]);
        Database_write(conn);
        break;
      case 'd':
        if (argc != 4)
          die("Need id to delete. ", conn);
    
        Database_delete(conn, id);
        Database_write(conn);
        break;
      case 'l':
        Database_list(conn);
        break;
      default:
        die("Invaild action: c=create, g=get, s=set, d=del, l=list", conn);
      }
      Database_close(conn);
    
      return 0;
    }
    ```
    
- 修改这段代码，让它接受MAX_DATA和MAX_ROWS的参数传入，将他们存储在Database结构体中，并写到文件中去，这样就能创建任意大小的数据库。
    
    ```c
    #include <stdio.h>
    #include <assert.h>
    #include <stdlib.h>
    #include <errno.h>
    #include <string.h>
     
    //#define MAX_DATA 512
    //#define MAX_ROWS 100
     
    struct Address {
        int id;
        int set;
        char *name;
        char *email;
    };
     
    struct Database {
        int MAX_DATA;
        int MAX_ROWS;
        struct Address **rows; // USE ARRAY OF POINTERS
    };
     
    struct Connection {
        FILE *file;
        struct Database *db;
    };
     
    void die(const char *message)
    {
        if(errno) {
            perror(message);
        } else {
            printf("ERROR: %s\n", message);
        }
     
        exit(1);
    }
     
    void Address_print(struct Address *addr)
    {
        printf("%d %s %s\n", addr->id, addr->name, addr->email);
    }
     
    void Database_load(struct Connection *conn)
    {
        size_t i=0;
        
        // Each database will have two `int` values. read
        // those first. 
        assert(conn->db && conn->file);
        if (!(conn->db && conn->file))
            die("Database load : Invalid Connection info");
        if (fread(&conn->db->MAX_DATA, sizeof(conn->db->MAX_DATA), 1, conn->file) != 1)
            die("Database load : Couldn't read MAX_DATA");
        if (fread(&conn->db->MAX_ROWS, sizeof(conn->db->MAX_ROWS), 1, conn->file) != 1)
            die("Database load : Couldn't read MAX_ROWS");
        conn->db->rows = (struct Address**)malloc(sizeof(struct Address *) * conn->db->MAX_ROWS);
    //  printf("Sizeof *conn->db->rows is %lu\n", sizeof(*conn->db->rows));
        assert(conn->db->rows);
        if (!(conn->db->rows))
            die("Database_load : Could not MAX_ROWS Address  structures");
        // Now load each address structure
        for (i=0; i<conn->db->MAX_ROWS; i++) {
            // read two int values first.
     
            conn->db->rows[i] = (struct Address*)malloc(sizeof(struct Address));
            struct Address *row = conn->db->rows[i];
            if (fread(&row->id, sizeof(row->id), 1, conn->file) !=1)
                die("Database load : Could not read Address::id");
            if (fread(&row->set, sizeof(row->set), 1, conn->file) != 1)
                die("Database load : Couldn't read  Address::set");
            // allocate two buffers large enough for our data
            row->name = malloc(conn->db->MAX_DATA * sizeof(*row->name));
            row->email = malloc(conn->db->MAX_DATA * sizeof(*row->email));
            assert(row->email && row->name);
            if (!(row->name && row->email))
                die("Database load : Failed to Allocate Address strings");
            // now just read both strings
            if (fread(row->name, conn->db->MAX_DATA * sizeof(*row->name), 1, conn->file) != 1)
                die("Database load : Failed to read Address::name");
            if (fread(row->email, conn->db->MAX_DATA * sizeof(*row->email), 1, conn->file) != 1)
                die("Database load : Failed to read Address::email");
        }
    }
     
    struct Connection *Database_open(const char *filename, char mode)
    {
        struct Connection *conn = malloc(sizeof(struct Connection));
        if(!conn) die("Memory error");
     
        conn->db = malloc(sizeof(struct Database));
        if(!conn->db) die("Memory error");
        
        if(mode == 'c') {
            conn->file = fopen(filename, "w");
        } else {
            conn->file = fopen(filename, "r+");
     
            if(conn->file) {
                Database_load(conn);
            }
        }
     
        if(!conn->file) die("Failed to open the file");
     
        return conn;
    }
     
    void Database_close(struct Connection *conn)
    {
            size_t i;
            if(conn) {
                    if (conn->db && conn->db->rows) {
                            for (i = 0; i < conn->db->MAX_ROWS; i++) {
     
                                    struct Address *cur = conn->db->rows[i];
                                    free(cur);
     
                            }
                    }
     
                    if(conn->file) fclose(conn->file);
                    if(conn->db) free(conn->db);
                    free(conn);
            }
    }
     
     
    void Database_write(struct Connection *conn)
    {
        size_t i = 0;
        rewind(conn->file);
        // Writing MAX_DATA and MAX_ROWS to the file first
        if(fwrite(&conn->db->MAX_DATA, sizeof(conn->db->MAX_DATA), 1, conn->file) != 1)
            die("Database write : Failed to write MAX_DATA");
        if(fwrite(&conn->db->MAX_ROWS, sizeof(conn->db->MAX_ROWS), 1, conn->file) != 1)
            die("Database write : Failed to write MAX_ROWS");
        // Writing Address's list now
        for (i=0; i<conn->db->MAX_ROWS; i++) {
            struct Address *row = conn->db->rows[i];
            if (fwrite(&row->id, sizeof(row->id), 1, conn->file) != 1)
                die("Database write : Failed to write Address(id)");
            if (fwrite(&row->set, sizeof(row->set), 1, conn->file) != 1)
                die("Database write : Failed to write Address(set)");
            if (fwrite(row->name, sizeof(char)*conn->db->MAX_DATA, 1, conn->file) != 1)
                die("Database write : Failed to write Address(name)");
            if (fwrite(row->email, sizeof(char)*conn->db->MAX_DATA, 1, conn->file) != 1)
                die("Database write : Failed to write Address(email)");
        }
        if (fflush(conn->file) == -1)
            die("Database_write : Cannot flush database.");
    }
     
    void Database_create(struct Connection *conn, int MAX_DATA, int MAX_ROWS)
    {
            int i = 0;
            conn->db->MAX_DATA = MAX_DATA;
            conn->db->MAX_ROWS = MAX_ROWS;
            conn->db->rows = (struct Address**)malloc(sizeof(struct Address*) * MAX_ROWS);
     
            for(i = 0; i < MAX_ROWS; i++) {
                    conn->db->rows[i] = (struct Address*)malloc(sizeof(struct Address));
                    conn->db->rows[i]->id = i;
                    conn->db->rows[i]->set = 0;
                    conn->db->rows[i]->name = (char *)malloc(conn->db->MAX_DATA);
                    conn->db->rows[i]->name = (char *)memset(conn->db->rows[i]->name, ' ', conn->db->MAX_DATA);
                    conn->db->rows[i]->email= (char *)malloc(conn->db->MAX_DATA);
                    conn->db->rows[i]->email= (char *)memset(conn->db->rows[i]->email, ' ', conn->db->MAX_DATA);
     
            }
    }
     
    void Database_set(struct Connection *conn, int id, const char *name, const char *email)
    {
        if (!(conn && conn->db && conn->db->rows && conn->db->rows[id])) return;
     
        struct Address *addr = conn->db->rows[id];
        int MAX_DATA = conn->db->MAX_DATA;
        if(addr->set == 1) die("Already set, delete it first");
     
        addr->set = 1;
        addr->name = malloc(sizeof(char) * MAX_DATA);
        addr->email = malloc(sizeof(char) * MAX_DATA);
     
        char *res = strncpy(addr->name, name, MAX_DATA);
        if(!res) die("Name copy failed");
     
        res = strncpy(addr->email, email, MAX_DATA);
        if(!res) die("Email copy failed");
    }
     
    void Database_get(struct Connection *conn, int id)
    {
        struct Address *addr = conn->db->rows[id];
     
        if(addr->set) {
            Address_print(addr);
        } else {
            die("ID is not set");
        }
    }
     
    void Database_delete(struct Connection *conn, int id)
    {
        struct Address addr = {.id = id, .set = 0};
        conn->db->rows[id] = &addr;
    }
     
    void Database_list(struct Connection *conn)
    {
        int i = 0;
        struct Database *db = conn->db;
        int MAX_ROWS = conn->db->MAX_ROWS;
        for(i = 0; i < MAX_ROWS; i++) {
            struct Address *cur = db->rows[i];
     
            if(cur->set) {
                Address_print(cur);
            }
        }
    }
     
    int main(int argc, char *argv[])
    {
        if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]");
     
        char *filename = argv[1];
        char action = argv[2][0];
        struct Connection *conn = Database_open(filename, action);
        int id = 0;
     
    //    if(argc > 3) id = atoi(argv[3]);
     //   if(id >= MAX_ROWS) die("There's not that many records.");
     
        switch(action) {
            case 'c':
            if(argc < 5) die("Required: MAX_DATA, MAX_ROWS");
                Database_create(conn, atoi(argv[3]), atoi(argv[4]));
                Database_write(conn);
                break;
     
            case 'g':
                if(argc != 4) die("Need an id to get");
     
                Database_get(conn, atoi(argv[3]));
                break;
     
            case 's':
                if(argc != 6) die("Need id, name, email to set");
                Database_set(conn, atoi(argv[3]), argv[4], argv[5]);
                Database_write(conn);
                break;
     
            case 'd':
                if(argc != 4) die("Need id to delete");
     
                Database_delete(conn, id);
                Database_write(conn);
                break;
     
            case 'l':
                Database_list(conn);
                break;
            default:
                die("Invalid action, only: c=create, g=get, s=set, d=del, l=list");
        }
     
        Database_close(conn);
     
        return 0;
    }
    ```