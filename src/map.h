typedef void* map;
map *initMap();
void addToMap(map *dest, void *key, void *value, int Ksize, int Vsize);
void *getFromMap(map *from, void *a, int size);
