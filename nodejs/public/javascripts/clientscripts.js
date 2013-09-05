var isConnected = false;
var socket = io.connect('http://192.168.1.139', {
    'reconnection delay': 1000,
    'max reconnection attempts': 99999999
});

socket.on('connect', function(){
    isConnected = true;
    console.log('connected');
    $('.ui-dialog').dialog('close');
    socket.emit('getPageState', {pageId: $.mobile.activePage.id});
});
socket.on('disconnect', function (){
    console.log('disconnected');
    $.mobile.changePage('#dialog', 'pop', true, true);
});
socket.on('close', function (){console.log('closed')});
socket.on('reconnect', function(){console.log('reconnected')});
socket.on('reconnecting', function(){console.log('reconnecting..')});

$(document).bind('pageinit', function() {
    console.log('Init sliders' + $('.slider'));
    //$('.slider').slider(); 
    //$("select:jqmData(role='slider')").slider(); 
    //$("select:jqmData(role='content')").trigger('create'); 
    //$("select:jqmData(role='slider')").trigger('create'); 
    //$("select:jqmData(role='slider')").slider('refresh'); 

    if (isConnected) {
        $('.ui-dialog').dialog('close');
    }
    $("select:jqmData(role='slider')").on( "change", function(event, ui) {
        console.log("Sending " + $(this).attr('id') + " " + $(this).val());
        socket.emit('flip', { flipid: $(this).attr('id'), state: $(this).val() });
    });
});

$(document).bind('pageinit', function(event) {
    //var activePage = $(event.target);
    console.log('pageinit for ' + event.target.id);
    socket.emit('getPageState', {pageId: event.target.id});
});
/*$(document).on("pagebeforeshow", "#lightsPage",function(event){
    alert('about to show page...');
    socket.emit('getPageState', $(event.target).id);
});*/

socket.on('flip', function (data) {
    console.log(data);
    if ($("#lightsPage").is(".ui-page-active")) {
        $('#' + data.flipid).val(data.state).slider('refresh');
    }
});
socket.on('temp', function (data) {
    console.log(data);
    $('#' + data.tempid).text(data.value);
    //socket.emit('my other event', { my: 'data' });
});
