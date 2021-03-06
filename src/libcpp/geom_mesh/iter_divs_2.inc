
    /*
    --------------------------------------------------------
     * ITER-DIVS-2: optim. schemes to split edges.
    --------------------------------------------------------
     *
     * This program may be freely redistributed under the 
     * condition that the copyright notices (including this 
     * entire header) are not removed, and no compensation 
     * is received through use of the software.  Private, 
     * research, and institutional use is free.  You may 
     * distribute modified versions of this code UNDER THE 
     * CONDITION THAT THIS CODE AND ANY MODIFICATIONS MADE 
     * TO IT IN THE SAME FILE REMAIN UNDER COPYRIGHT OF THE 
     * ORIGINAL AUTHOR, BOTH SOURCE AND OBJECT CODE ARE 
     * MADE FREELY AVAILABLE WITHOUT CHARGE, AND CLEAR 
     * NOTICE IS GIVEN OF THE MODIFICATIONS.  Distribution 
     * of this code as part of a commercial system is 
     * permissible ONLY BY DIRECT ARRANGEMENT WITH THE 
     * AUTHOR.  (If you are not directly supplying this 
     * code to a customer, and you are instead telling them 
     * how they can obtain it for free, then you are not 
     * required to make any arrangement with me.) 
     *
     * Disclaimer:  Neither I nor: Columbia University, The
     * Massachusetts Institute of Technology, The 
     * University of Sydney, nor The National Aeronautics
     * and Space Administration warrant this code in any 
     * way whatsoever.  This code is provided "as-is" to be 
     * used at your own risk.
     *
    --------------------------------------------------------
     *
     * Last updated: 17 November, 2017
     *
     * Copyright 2013-2017
     * Darren Engwirda
     * de2363@columbia.edu
     * https://github.com/dengwirda/
     *
    --------------------------------------------------------
     */
    
    // from iter_mesh_2.hpp
    

    /*
    --------------------------------------------------------
     * _DIV-EDGE: try edge split to improve adj. cost-fun.
    --------------------------------------------------------
     */
 
    __static_call
    __normal_call void_type _div_edge (
        geom_type &_geom ,
        mesh_type &_mesh , 
        size_type &_hfun ,
        pred_type &_pred ,
        iptr_type  _enum ,
        bool_type &_okay ,
        iptr_list &_tset ,
        real_list &_csrc ,
        real_list &_cdst ,
        real_type  _lmin
            = (real_type) +7.50E-01,
        real_type  _good 
            = (real_type) +9.00E-01,
        real_type  _qinc 
            = (real_type) +1.00E-08
        )
    {    
         auto _edge = 
        _mesh._set2.head() + _enum ;
        
		__unreferenced(_pred) ; // for MSVC...

        _okay = false ;
        
        iptr_type _enod[2] ;
        _enod[ 0] = _edge->node(0) ;
        _enod[ 1] = _edge->node(1) ;
        
         auto _iptr = _mesh.
        _set1.head()+ _edge->node(0) ;        
         auto _jptr = _mesh.
        _set1.head()+ _edge->node(1) ;
        
        _tset.set_count(+0);
        _mesh.edge_tri3(_enum, _tset);
    
        if (_tset.count()!=2) return ;
    
        real_type _ipos[_dims] ;
        real_type _jpos[_dims] ;
        iptr_type _idim = +0;
        for (_idim = _dims+0; _idim-- != 0; )
        {
            _ipos[_idim] =
                _iptr->pval(_idim) ;
            _jpos[_idim] =
                _jptr->pval(_idim) ;
        }

        real_type  _isiz = 
        _hfun.eval(_ipos, _iptr->hidx ()) ;
        real_type  _jsiz = 
        _hfun.eval(_jpos, _jptr->hidx ()) ;
        
        real_type  _elen = std::
        sqrt(_pred.length_sq(_ipos,_jpos) ) ;

        real_type  _hbar = 
       (real_type) (1./2.) * _isiz + 
       (real_type) (1./2.) * _jsiz ;

        if (_elen <= _hbar * _lmin)
            return  ;
         
         auto _itri = 
        _mesh._set3.head()+_tset[0];
         auto _jtri = 
        _mesh._set3.head()+_tset[1];
    
        _csrc.set_count(+2) ;
        _cdst.set_count(+4) ;
    
        _csrc[0] = _pred.cost_tria (
           &_mesh._set1[
            _itri->node(0)].pval(0),
           &_mesh._set1[
            _itri->node(1)].pval(0),
           &_mesh._set1[
            _itri->node(2)].pval(0)
            ) ;
     
        _csrc[1] = _pred.cost_tria (
           &_mesh._set1[
            _jtri->node(0)].pval(0),
           &_mesh._set1[
            _jtri->node(1)].pval(0),
           &_mesh._set1[
            _jtri->node(2)].pval(0)
            ) ;
            
        if (_csrc[1] < _csrc[0])
        std::swap(_itri, _jtri ) ;
        
        real_type _cmin = std::min (
            _csrc[0] , _csrc[1]) ;
        
        if (_cmin > _good) return;
        
        real_type _ibal[_dims+1] ;
        _pred.mini_ball(_ibal ,
           &_mesh._set1[
            _itri->node(0)].pval(0),
           &_mesh._set1[
            _itri->node(1)].pval(0),
           &_mesh._set1[
            _itri->node(2)].pval(0)
            ) ;

        real_type _ipwr = (real_type)+0.;
        _ipwr += _mesh._set1[
            _itri->node(0)].pval(_dims) ;
        _ipwr += _mesh._set1[
            _itri->node(1)].pval(_dims) ;
        _ipwr += _mesh._set1[
            _itri->node(2)].pval(_dims) ;
        _ipwr /= (real_type) +3. ;

        real_type _jbal[_dims+1] ;
        _pred.mini_ball(_jbal ,
           &_mesh._set1[
            _jtri->node(0)].pval(0),
           &_mesh._set1[
            _jtri->node(1)].pval(0),
           &_mesh._set1[
            _jtri->node(2)].pval(0)
            ) ;
        
        real_type _jpwr = (real_type)+0.;
        _jpwr += _mesh._set1[
            _jtri->node(0)].pval(_dims) ;
        _jpwr += _mesh._set1[
            _jtri->node(1)].pval(_dims) ;
        _jpwr += _mesh._set1[
            _jtri->node(2)].pval(_dims) ;
        _jpwr /= (real_type) +3. ;
        
        iptr_type _inod [3] ;
        iptr_type _jnod [3] ;
        for(auto _inum = 3; _inum-- != 0; )
        {
            mesh_type::tri3_type::
            face_node(_inod, _inum, 2, 1) ;
            _inod[0] = 
                _itri->node(_inod[0]);
            _inod[1] = 
                _itri->node(_inod[1]);
            _inod[2] = 
                _itri->node(_inod[2]);
        
            if (_inod[2] != _enod[0])
            if (_inod[2] != _enod[1])
                break ;
        }
        for(auto _inum = 3; _inum-- != 0; )
        {
            mesh_type::tri3_type::
            face_node(_jnod, _inum, 2, 1) ;
            _jnod[0] = 
                _jtri->node(_jnod[0]);
            _jnod[1] = 
                _jtri->node(_jnod[1]);
            _jnod[2] = 
                _jtri->node(_jnod[2]);
        
            if (_jnod[2] != _enod[0])
            if (_jnod[2] != _enod[1])
                break ;
        }
        
        real_type _ball[_dims+1] = 
        {   (real_type) +0.000 } ;
        real_type _circ[_dims+1] = 
        {   (real_type) +0.000 } ;
        
        iptr_type static 
            constexpr _INUM = (iptr_type)+16 ;
        
        _ball[_dims+0] = 
        (real_type)+.5 * _ipwr +
        (real_type)+.5 * _jpwr ;
        
        for (auto _iloc = +0; _iloc != _INUM ; 
                ++_iloc )
        {   
            real_type _bias = 
           (real_type)_iloc / (_INUM-1) ;
            
            real_type _apos = 
                (real_type)+0. + _bias ;    
            real_type _bpos = 
                (real_type)+1. - _bias ;
        
            for(_idim = _dims+0; _idim-- != 0; )
            {
                _ball[_idim] = 
                  _apos * _jbal[_idim] +
                  _bpos * _ibal[_idim] ;
            }
            
            _pred. proj_node(
                _geom, _circ, _ball) ;
            
            _cdst[0] = _pred.cost_tria (
                _ball, &_mesh.
            _set1[_inod[2]].pval(0),
               &_mesh.
            _set1[_inod[0]].pval(0)
                ) ;
            _cdst[1] = _pred.cost_tria (
                _ball, &_mesh.
            _set1[_inod[1]].pval(0),
               &_mesh.
            _set1[_inod[2]].pval(0)
                ) ;
               
            _cdst[2] = _pred.cost_tria (
                _ball, &_mesh.
            _set1[_jnod[1]].pval(0),
               &_mesh.
            _set1[_jnod[2]].pval(0)
                ) ;
            _cdst[3] = _pred.cost_tria (
                _ball, &_mesh.
            _set1[_jnod[2]].pval(0),
               &_mesh.
            _set1[_jnod[0]].pval(0)
                ) ;
               
            move_okay(_cdst, _csrc , 
                      _okay, _good , 
                      _qinc) ;
             
            if (_okay) break ;         
        }
        
        if (!_okay) return ;     
    
    
        typename mesh_type
               ::node_type _ndat ;
        typename mesh_type
               ::tri3_type _tdat ;
        
        _ndat.hidx() = 
            size_type::null_hint() ;
               
        for(_idim = _dims+1; _idim-- != 0; )
        {
            _ndat.pval(_idim) = 
                    _ball[_idim] ;
        } 
         
        iptr_type _nnew = 
            _mesh.push_node(_ndat) ;
               
        _mesh._pop_tri3(_tset[0]);
        _mesh._pop_tri3(_tset[1]);
               
        _tdat.node(0) = _nnew;
        _tdat.node(1) = _inod[2] ;
        _tdat.node(2) = _inod[0] ;

        _mesh.push_tri3(_tdat) ;
        
        _tdat.node(0) = _nnew;
        _tdat.node(1) = _inod[1] ;
        _tdat.node(2) = _inod[2] ;
        
        _mesh.push_tri3(_tdat) ;
        
        _tdat.node(0) = _nnew;
        _tdat.node(1) = _jnod[1] ;
        _tdat.node(2) = _jnod[2] ;
        
        _mesh.push_tri3(_tdat) ;
        
        _tdat.node(0) = _nnew;
        _tdat.node(1) = _jnod[2] ;
        _tdat.node(2) = _jnod[0] ;
        
        _mesh.push_tri3(_tdat) ;
    }
    
    
    
