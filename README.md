# n_sliding_puzzle_astar

### cél állapot:  
1 2 3  
4 5 6  
7 8 0

### Implementált parancssori kapcsolók:  
- -input input.txt (start állapot beolvasása, ha nincs billentyűzetről várja)
- -solseq (kiírja a lépéseket)
- -pcost (lépések száma)
- -nvisited (meglátogatott csomópontok száma)
- -h 1 / -h 2 (1 esetén a rossz elemek száma heurisztikát, 2 esetén a manhattan heurisztikát használja)

### Heurisztiák összehasonlítása:
1 6 2  
0 4 3  
7 5 8  
- ./puzzle -input input.txt -nvisited -pcost -h 1  
Megoldás költsége: 7  
Meglátogatott csomópontok száma: 12  
- ./puzzle -input input.txt -nvisited -pcost -h 2  
Megoldás költsége: 7  
Meglátogatott csomópontok száma: 10  
  
2 1 3  
5 4 0  
6 7 8  
- ./puzzle -input input2.txt -nvisited -pcost -h 1  
Megoldás költsége: 19  
Meglátogatott csomópontok száma: 2539  
- ./puzzle -input input2.txt -nvisited -pcost -h 2  
Megoldás költsége: 19  
Meglátogatott csomópontok száma: 501  
