 ll c[4] = {lca(a,x), lca(a,y), lca(b,x), lca(b,y)};
  sort(c, c+4, [](int p, int q){ return depth[p] > depth[q]; });
  int u = c[0], v = c[1];
  ll tr=0;
  if(dist(a,u)+dist(u,b) != dist(a,b) || dist(x,u)+dist(u,y) != dist(x,y) ||
      dist(a,v)+dist(v,b) != dist(a,b) || dist(x,v)+dist(v,y) != dist(x,y)){
      tr=1;
   }

a = u; b = v;  // a->b is the common path
