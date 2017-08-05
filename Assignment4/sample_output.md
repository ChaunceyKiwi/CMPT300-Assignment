## Sample 1
### Input
`./UnixLs -l`
### ls output
```
-rwxr-xr-x 1 cla284 domain users   415 Aug  3 18:57 Makefile
-rwxr-xr-x 1 cla284 domain users 19544 Aug  4 19:46 UnixLs
-rw-r--r-- 1 cla284 domain users 20232 Aug  4 19:46 UnixLs.o
```

### UnixLs output
```
-rwxr-xr-x  1   cla284 domain users    415 Aug  3 2017 18:57 Makefile
-rw-r--r--  1   cla284 domain users  20232 Aug  4 2017 19:46 UnixLs.o
-rwxr-xr-x  1   cla284 domain users  19544 Aug  4 2017 19:46 UnixLs
```

## Sample 2
### Input
`./UnixLs -l ../test/a/a3-symbol.txt`

### ls output
```
lrwxrwxrwx 1 cla284 domain users 6 Aug  3 18:57 ../test/a/a3-symbol.txt -> a3.txt
```

### UnixLs output
```
lrwxrwxrwx  1   cla284 domain users      6 Aug  3 2017 18:57 ../test/a/a3-symbol.txt -> a3.txt
```

## Sample 3
### Input
`./UnixLs -ilR ../test`

### ls output
```
../test:
total 0
 111821193 drwxr-sr-x 4 cla284 domain users 105 Aug  3 18:57 a
1623208555 drwxr-sr-x 4 cla284 domain users  53 Aug  3 18:57 b

../test/a:
total 4
 688026783 drwxr-sr-x 2 cla284 domain users 34 Aug  3 18:57 a1
1205202547 drwxr-sr-x 2 cla284 domain users 34 Aug  3 18:57 a2
 111881352 lrwxrwxrwx 1 cla284 domain users  2 Aug  3 18:57 a2-dir-symbol.txt -> a2
 111881353 lrwxrwxrwx 1 cla284 domain users  6 Aug  3 18:57 a3-symbol.txt -> a3.txt
 111881354 -rw-r--r-- 1 cla284 domain users  6 Aug  3 18:57 a3.txt

../test/a/a1:
total 8
688041276 -rw-r--r-- 1 cla284 domain users 6 Aug  3 18:57 a11.txt
688041277 -rw-r--r-- 1 cla284 domain users 6 Aug  3 18:57 a12.txt

../test/a/a2:
total 8
1205202582 -rw-r--r-- 1 cla284 domain users 6 Aug  3 18:57 a21.txt
1205240808 -rw-r--r-- 1 cla284 domain users 6 Aug  3 18:57 a22.txt

../test/b:
total 4
2173719225 drwxr-sr-x 2 cla284 domain users 34 Aug  3 18:57 b1
2743574898 drwxr-sr-x 2 cla284 domain users 34 Aug  3 18:57 b2
1630468805 -rw-r--r-- 1 cla284 domain users  6 Aug  3 18:57 b3.txt

../test/b/b1:
total 8
2176952511 -rw-r--r-- 1 cla284 domain users 6 Aug  3 18:57 b11.txt
2177946470 -rw-r--r-- 1 cla284 domain users 6 Aug  3 18:57 b12.txt

../test/b/b2:
total 8
2743574901 -rw-r--r-- 1 cla284 domain users 6 Aug  3 18:57 b21.txt
2743574902 -rw-r--r-- 1 cla284 domain users 6 Aug  3 18:57 b22.txt
```

### UnixLs output
```
../test:
111821193 drwxr-xr-x  4   cla284 domain users    105 Aug  3 2017 18:57 a
1623208555 drwxr-xr-x  4   cla284 domain users     53 Aug  3 2017 18:57 b

../test/a:
688026783 drwxr-xr-x  2   cla284 domain users     34 Aug  3 2017 18:57 a1
111881352 lrwxrwxrwx  1   cla284 domain users      2 Aug  3 2017 18:57 a2-dir-symbol.txt -> a2
1205202547 drwxr-xr-x  2   cla284 domain users     34 Aug  3 2017 18:57 a2
111881353 lrwxrwxrwx  1   cla284 domain users      6 Aug  3 2017 18:57 a3-symbol.txt -> a3.txt
111881354 -rw-r--r--  1   cla284 domain users      6 Aug  3 2017 18:57 a3.txt

../test/a/a1:
688041276 -rw-r--r--  1   cla284 domain users      6 Aug  3 2017 18:57 a11.txt
688041277 -rw-r--r--  1   cla284 domain users      6 Aug  3 2017 18:57 a12.txt

../test/a/a2:
1205202582 -rw-r--r--  1   cla284 domain users      6 Aug  3 2017 18:57 a21.txt
1205240808 -rw-r--r--  1   cla284 domain users      6 Aug  3 2017 18:57 a22.txt

../test/b:
2173719225 drwxr-xr-x  2   cla284 domain users     34 Aug  3 2017 18:57 b1
2743574898 drwxr-xr-x  2   cla284 domain users     34 Aug  3 2017 18:57 b2
1630468805 -rw-r--r--  1   cla284 domain users      6 Aug  3 2017 18:57 b3.txt

../test/b/b1:
2176952511 -rw-r--r--  1   cla284 domain users      6 Aug  3 2017 18:57 b11.txt
2177946470 -rw-r--r--  1   cla284 domain users      6 Aug  3 2017 18:57 b12.txt

../test/b/b2:
2743574901 -rw-r--r--  1   cla284 domain users      6 Aug  3 2017 18:57 b21.txt
2743574902 -rw-r--r--  1   cla284 domain users      6 Aug  3 2017 18:57 b22.txt
```

