use std::cmp::max;
use std::sync::mpsc;
use std::thread;
use rand::Rng;

use nalgebra::DMatrix;

struct Graphe_edges {
    nb_v:usize,
    edges:Vec<(usize,usize)>,
}

struct Graphe_matrix {
    data:DMatrix<bool>
}

impl Graphe_edges {
    pub fn new(vec:Vec<(usize,usize)>) -> Self {

        let m = vec.iter().fold(0, |a,(i,j)| max(*i,max(*j,a)) );
        Graphe_edges {edges:vec,nb_v:m}
    }
    pub fn random(nb_v:usize, nb_e:usize) -> Self {
        loop {

        }
        Graphe_edges {edges:vec,nb_v:m}
    }
}

impl Graphe_matrix {
    pub fn new(mat:DMatrix<bool>) -> Self {
        Graphe_matrix {data:mat}
    }
    pub fn from_edge(ed:&Graphe_edges) -> Self {
        let data = DMatrix::from_fn(ed.nb_v,ed.nb_v,
            |x1,y1| ed.edges.iter().any(|(i,j)| *i==x1 && *j==y1) );
        Self::new(data)
    }
}


fn main() {
    let mut rng = rand::thread_rng();


}