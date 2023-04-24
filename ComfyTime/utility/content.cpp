const char HOME_PAGE[]  = R"=====(
<!DOCTYPE html>
<html>
<style>
.card{
    max-width: 400px;
     min-height: 250px;
     background: #02b875;
     padding: 30px;
     box-sizing: border-box;
     color: #FFF;
     margin:20px;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
}
</style>

<body>

<div class="card">
  <h4>Room Comfy-ness</h4><br>
  <h1>Light:<span id="lightValue">0</span></h1><br>  
  <h1>Temperature:<span id="tempValue">0</span></h1><br>  
  <h1>Humidity:<span id="humValue">0</span></h1><br>  
</div>

<script>
// The span above has ID as ADCValue - its value/content can be replaced programatically.

// every 2000m Seconds call the function 'getData()'
// set interval - is built-in method, takes 
// lambda function, which calls getData(), and an int value as the time limit
//----------------------------------------------------
setInterval(function() {getLight();}, 2000);
setInterval(function() {getTemp();}, 2000);
setInterval(function() {getHum();}, 2000);

// This function is called by the setInterval function.
// Declare an XMLHttpRequest() object - xhttp
// get value from readADc url, package and send it back to user.
function getLight() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {  // if ready and staus is 200
      document.getElementById("lightValue").innerHTML = // find the ADCValue span above and change its value.
      this.responseText;
    }
  };

  xhttp.open("GET", "readLight", true); // GET request to readADC url, which is the current value of the sensor.
  xhttp.send();                       // send the request - after changing the vlaues in html and getting 200 codes data is sent.
}

function getTemp() {
  var tempxhttp = new XMLHttpRequest();

  tempxhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {  // if ready and staus is 200
      document.getElementById("tempValue").innerHTML = // find the tempValue span above and change its value.
      this.responseText;
    }
  };
  tempxhttp.open("GET", "readTemperature", true);
  tempxhttp.send();  // send the request - after changing the vlaues in html and getting 200 codes data is sent.
}


function getHum() {
  var tempxhttp = new XMLHttpRequest();

  tempxhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {  // if ready and staus is 200
      document.getElementById("humValue").innerHTML = // find the tempValue span above and change its value.
      this.responseText;
    }
  };

  tempxhttp.open("GET", "readHumidity", true);
  tempxhttp.send();  // send the request - after changing the vlaues in html and getting 200 codes data is sent.
}


</script>
</body>
</html>

)=====";