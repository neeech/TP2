#include "Servidor.h"

Servidor::Servidor(Nat cantJug, Variante v, Repositorio rep): juego(cantJug,v,rep){
    _clientes.esperados = cantJug;
    _clientes.conectados = 0;
    vector<tuple<Notificacion,Nat>> vacio = {};
    vector<vector<tuple<Notificacion,Nat>>> porJugador(cantJug, vacio) ;
    _notifs.notificacionesPorJugador = porJugador;
    _notifs.nTodes.notificacionesServidor = {};
    vector<int> leyoHasta(cantJug,0);
    _notifs.nTodes.leyoHasta = leyoHasta;
    _idNotif = 0;

}

void Servidor::conectarCliente() { //TODO : mandar fichas como en el mail


    _notifs.notificacionesPorJugador[_clientes.conectados].push_back(make_tuple(Notificacion ::nuevaIdCliente(_clientes.conectados),_idNotif));
    _idNotif++;
    if (_clientes.conectados == _clientes.esperados){
        _notifs.nTodes.notificacionesServidor.push_back(make_tuple(Notificacion ::nuevaEmpezar(juego.tamanoTab()),_idNotif));
        _notifs.nTodes.notificacionesServidor.push_back(make_tuple(Notificacion::nuevaTurnoDe(0), ++ _idNotif));
        _idNotif++;
    }
    _clientes.conectados ++;


}

void Servidor::recibirMensaje(Ocurrencia o, Nat idCliente) {

    bool valida  = juego.EsjugadaValida(o);
    bool tieneFichas = juego.tieneFichas(o,idCliente);
    bool esTurno = juego.obtenerTurno() == idCliente;
    bool comenzo = numeroClientesEsperados() == numeroClientesConectados();
    if(valida and tieneFichas and esTurno and comenzo){
        Nat viejoPuntaje = juego.puntajeAnterior(idCliente);
        juego.ubicar(o);
        _notifs.nTodes.notificacionesServidor.push_back(
                make_tuple(Notificacion ::nuevaSumaPuntos(idCliente,
                                                                   juego.obtenerPuntaje(idCliente) - viejoPuntaje )
                                                                   ,_idNotif));
        _notifs.nTodes.notificacionesServidor.push_back(make_tuple(Notificacion::nuevaUbicar(idCliente,o),++_idNotif));
        _notifs.notificacionesPorJugador[idCliente].push_back(make_tuple(Notificacion::nuevaReponer(juego.reponerN(o.size(),idCliente)),++ _idNotif));
        _notifs.nTodes.notificacionesServidor.push_back(make_tuple(Notificacion ::nuevaTurnoDe(idCliente + 1 ),++ _idNotif));
        _idNotif++;


    }
    else{
        _notifs.notificacionesPorJugador[idCliente].push_back(make_tuple(Notificacion :: nuevaMal(),_idNotif ++));
    }
}

const Nat Servidor::numeroClientesEsperados() const {
    return 0;
}

const Nat Servidor::numeroClientesConectados() const {
    return 0;
}

void Servidor::consultarNotificaciones(Nat idCliente) {

}