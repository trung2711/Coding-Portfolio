/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
//Create an object to make thing less global
var arr = [];
var slideIndex = 1;
var modal = document.createElement("div");
modal.id = "myModal";
var numImg = 0;

function addImgArr(arrAdd){
    for (var i = 0; i<arrAdd.length; i++){
        arr[numImg++] = arrAdd[i];
    }
}

function clearImg(){
    arr = [];
    numImg = 0;
}

function addImg(path){
    arr.push(path);
    numImg++;
}
function removeImg(path){
   for(var i = 0; i<arr.length; i++){
        if (arr[i] === path){
            arr.splice(i,1);
            numImg--;
        }
    } 
}

function createModal(assignButtonId, bgId, demoHeight){
    modal.innerHTML = "";
    var bar = document.createElement("div");
    bar.id = "bar";
    var leftBtn = document.createElement("a");
    leftBtn.innerHTML = "&#10094;";
    leftBtn.className = "prev";
    leftBtn.setAttribute("onclick","plusSlides(-1)");
    var rightBtn = document.createElement("a");
    rightBtn.innerHTML = "&#10095;";
    rightBtn.className = "next";
    rightBtn.setAttribute("onclick","plusSlides(1)");
    bar.appendChild(leftBtn);
    bar.appendChild(rightBtn);
    for(var i = 0; i<numImg; i++){
        var column = document.createElement("div");
        column.className = "column";
        var newImg = document.createElement("img");
        newImg.src = arr[i];
        newImg.className = "demo cursor";
        newImg.alt = "Cursor Imgage " + (i+1);
        var funcName = "currentSlide("+(i+1)+")";
        newImg.setAttribute("onclick",funcName);
        column.appendChild(newImg);
        bar.appendChild(column);
    }
    var mainDiv = document.createElement("div");
    mainDiv.id = "container";
    for(var i = 0; i<numImg; i++){
        var newImg = document.createElement("img");
        newImg.src = arr[i];
        newImg.className = "mySlides";
        newImg.alt = "Slide Imgage " + (i+1);
        mainDiv.appendChild(newImg);
    }
    if(demoHeight!=null){
        bar.style.height = demoHeight+"px";
        var newPad = demoHeight/2-20;
        leftBtn.style.paddingTop = newPad+"px";
        rightBtn.style.paddingTop = newPad+"px";
        mainDiv.style.marginTop = (demoHeight+20)+"px";
    }
    var closeBtn = document.createElement("div");
    closeBtn.innerHTML = "&times;";
    closeBtn.className = "close center";
    closeBtn.setAttribute("onclick","hideDisplay()");
    mainDiv.appendChild(closeBtn);
    modal.appendChild(bar);
    modal.appendChild(mainDiv);
    document.body.appendChild(modal);
    if(bgId!==null){
        document.getElementById(bgId).className += " bg";
        var setBgBtn = document.createElement("div");
        setBgBtn.className = "center";
        var bgBtn = document.createElement("button");
        bgBtn.innerHTML = "Set as Background";
        bgBtn.setAttribute("onclick","setBg('"+bgId+"')");
        setBgBtn.appendChild(bgBtn);
        mainDiv.appendChild(setBgBtn);
    }
    var btn = document.getElementById(assignButtonId);
    btn.setAttribute("onclick","openModal()");
    showSlides(slideIndex);
}

window.onclick = function(event) {
  if (event.target == modal) {
    modal.style.display = "none";
  }
}

function hideDisplay(){
    modal.style.display = "none";
}

function openModal(){
    modal.style.display = "block";
}

function plusSlides(n) {
  showSlides(slideIndex += n);
}

// Thumbnail image controls
function currentSlide(n) {
  showSlides(slideIndex = n);
}

function showSlides(n){
  var i;
  var slides = document.getElementsByClassName("mySlides");
  var dots = document.getElementsByClassName("demo");
  if (n > slides.length) {slideIndex = 1;}
  if (n < 1) {slideIndex = slides.length;}
  for (i = 0; i < slides.length; i++) {
    slides[i].style.display = "none";
  }
  for (i = 0; i < dots.length; i++) {
    dots[i].className = dots[i].className.replace(" active", "");
  }
  slides[slideIndex-1].style.display = "block";
  dots[slideIndex-1].className += " active";
}

function setBg(id){
  document.getElementById(id).style.backgroundImage = "url("+arr[slideIndex-1];+")";
}