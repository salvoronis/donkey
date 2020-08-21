typedef void* map;
map *initMap(int size);
void addToMap(map *dest, void *a, int size);
void *getFromMap(map *from, void *a, int size);
