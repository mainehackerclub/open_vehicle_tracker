function get_random_color() {
   var letters = '0123456789ABCDEF'.split('');
   var color = '#';
   for (var i = 0; i < 6; i++) {
       color += letters[Math.round(Math.random() * 15)];
   }
   return color;
}

function vehiclePopup(vehicle) {
   var vehicleId = vehicle.id;
   var popupWindow = new google.maps.InfoWindow();
   jQuery.getJSON("http://awesomesauce.me/ajax.php", {
           vehicleid: vehicleId
       })
       .done(function (data) {
           if (data == null) return;
           posish = new google.maps.LatLng(data[0].lat, data[0].lon);
           var place = latLongAddress(posish, vehicleId);
           var info = "ID: " + vehicleId +
               "\nCurrent Approximate Location: " + place +
               "\nSalt Rate: " +
               "\nSand Rate: ";
           popupWindow.setContent(info);
           popupWindow.open(map, markers[vehicleId - 1]);
           window.map.setCenter(posish);
       });
}

function getTruck(truck) {
   var value = truck.value;
   var id = truck.id;
   alert(id);
}

var geocoder = null;
var infowindow = new google.maps.InfoWindow();
//var marker = null;
function initialize_map() {
  geocoder = new google.maps.Geocoder();
  var map_canvas = document.getElementById('map_canvas');
  //map_canvas.style.height = $(window).height();
  $('#map_canvas').css('height', ($(window).height()));
  
  var mapOptions = {
    zoom: 12,
    center: new google.maps.LatLng(44.801207, -68.777817),
    disableDefaultUI: true,
    mapTypeId: google.maps.MapTypeId.ROADMAP
  };

  window.map = new google.maps.Map(map_canvas, mapOptions);
  markers.push = new google.maps.Marker({
    /*icon: new google.maps.MarkerImage('noun_project_4247.svg',
      null, null, null, new google.maps.Size(64,64)),*/
    map: map
  });


  google.maps.event.trigger(map, 'resize');
}

function in_circle(center_x, center_y, radius, x, y){
square_dist = (center_x - x) ^ 2 + (center_y - y) ^ 2
return (square_dist <= (radius ^ 2))
}

function latLongAddress(latLng, id) {
   geocoder.geocode({
           'latLng': latLng
       }, function (results, status) {
           if (status == google.maps.GeocoderStatus.OK) {
               if (results[0]) {
                    //map.setZoom(11);
                 markers[id].setPosition(latLng);
                   infowindow.setContent(results[0].formatted_address);
                   infowindow.open(map, markers[id]);
                   return results[0].formatted_address;
               }
           } else {
               alert("GeoCoder failed due to: " + status);
           }
       });
}
window.lastid = 0;

var markers = new Array();
var flightpaths = new Array();
var flightpathcoords = new Array();

function map_points() {
   jQuery.getJSON('http://awesomesauce.me/ajax.php?lastid=' + window.lastid + '&jsonp=?', function (data) {
     if (data) {
       var PreviousLatLong = null;
               jQuery.each(data, function (i, d) {
                 if (markers[d.truckid]) {
                   var ItemLatLong = new google.maps.LatLng(d.lat, d.lon);
                   if(PreviousLatLong !== null && google.maps.geometry.spherical.computeDistanceBetween(PreviousLatLong,ItemLatLong) < 15) return true;
                   PreviousLatLong = ItemLatLong;
                           markers[d.truckid].setPosition(ItemLatLong);
                           flightpathcoords[d.truckid].push(ItemLatLong);
                           flightpaths[d.truckid].setPath(flightpathcoords[d.truckid]);
                           flightpaths[d.truckid].setMap(window.map);
                 } else {
                   var ItemLatLong = new google.maps.LatLng(d.lat, d.lon);
                   if(PreviousLatLong !== null && google.maps.geometry.spherical.computeDistanceBetween(PreviousLatLong,ItemLatLong) < 15) return true;
                   PreviousLatLong = ItemLatLong;
                           markers[d.truckid] = new google.maps.Marker({
                                   position: ItemLatLong,
                                     title: "Snow Plow",
                                     
  /*icon: new google.maps.MarkerImage('noun_project_4247.svg',
  null, null, null, new google.maps.Size(64,64)),*/
                               });
                           flightpathcoords[d.truckid] = new Array();
                           flightpathcoords[d.truckid].push(ItemLatLong);
                           flightpaths[d.truckid] = new google.maps.Polyline({
                                   path: flightpathcoords[d.truckid],
                                   strokeColor: get_random_color(),
                                   strokeOpacity: 1.0,
                                   strokeWeight: 2
                               });
                           flightpaths[d.truckid].setMap(window.map);
                           markers[d.truckid].setMap(window.map);
                       }
                       window.lastid = d.lastid;
                   });
           }
       });


   function popToolbar(typeArray) {

   }



   setTimeout(function () {
           map_points();
       }, 3000);
}

function initialize () {
 //while(google.maps.geometry){
  initialize_map();
  map_points();
  google.maps.event.trigger(map, 'resize');
}

$(window).resize(function () {
      var h = $(window).height(),
        offsetTop = 5; // Calculate the top offset

    $('#map_canvas').css('height', (h - offsetTop));
}).resize();

google.maps.event.addDomListener(window, 'load', initialize);
