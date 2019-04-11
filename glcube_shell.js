var checkboxFunction;
var setDelayFunction;
var setAngles;
var statusElement = document.getElementById('status');
var progressElement = document.getElementById('progress');
var spinnerElement = document.getElementById('spinner');

var Module = {
	onRuntimeInitialized: function() {
		checkboxFunction=Module.cwrap('checkbox',null);
		setDelayFunction=Module.cwrap('setDelay',null,['int']);
		setAngles=Module.cwrap('setAngles',null,['double','double']);
	},
	preRun: [],
	postRun: [],
	print: (function() {
		var element = document.getElementById('output');
		if (element) element.value = ''; // clear browser cache
		return function(text) {
			if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
			// These replacements are necessary if you render to raw HTML
			//text = text.replace(/&/g, "&amp;");
			//text = text.replace(/</g, "&lt;");
			//text = text.replace(/>/g, "&gt;");
			//text = text.replace('\n', '<br>', 'g');
			console.log(text);
			if (element) {
				element.value += text + "\n";
				element.scrollTop = element.scrollHeight; // focus on bottom
			}
		};
	})(),
	printErr: function(text) {
		if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
		console.error(text);
	},
	canvas: (function() {
		var canvas = document.getElementById('canvas');

		// As a default initial behavior, pop up an alert when webgl context is lost. To make your
		// application robust, you may want to override this behavior before shipping!
		// See http://www.khronos.org/registry/webgl/specs/latest/1.0/#5.15.2
		canvas.addEventListener("webglcontextlost", function(e) { alert('WebGL context lost. You will need to reload the page.'); e.preventDefault(); }, false);

		return canvas;
	})(),
	setStatus: function(text) {
		if (!Module.setStatus.last) Module.setStatus.last = { time: Date.now(), text: '' };
		if (text === Module.setStatus.last.text) return;
		var m = text.match(/([^(]+)\((\d+(\.\d+)?)\/(\d+)\)/);
		var now = Date.now();
		if (m && now - Module.setStatus.last.time < 30) return; // if this is a progress update, skip it if too soon
		Module.setStatus.last.time = now;
		Module.setStatus.last.text = text;
		if (m) {
			text = m[1];
			progressElement.value = parseInt(m[2])*100;
			progressElement.max = parseInt(m[4])*100;
			progressElement.hidden = false;
			spinnerElement.hidden = false;
		} else {
			progressElement.value = null;
			progressElement.max = null;
			progressElement.hidden = true;
			if (!text) spinnerElement.hidden = true;
		}
		statusElement.innerHTML = text;
	},
	totalDependencies: 0,
	monitorRunDependencies: function(left) {
		this.totalDependencies = Math.max(this.totalDependencies, left);
		Module.setStatus(left ? 'Preparing... (' + (this.totalDependencies-left) + '/' + this.totalDependencies + ')' : 'All downloads complete.');
	}
};
Module.setStatus('Downloading...');
window.onerror = function() {
	Module.setStatus('Exception thrown, see JavaScript console');
	spinnerElement.style.display = 'none';
	Module.setStatus = function(text) {
		if (text) Module.printErr('[post-exception status] ' + text);
	};
};
var sliderX = document.getElementById("mySliderX");
var sliderOutputX = document.getElementById("slider_value_x");
sliderX.step="0.05"
sliderOutputX.innerHTML = sliderX.value; // Display the default slider value
var sliderY = document.getElementById("mySliderY");
var sliderOutputY = document.getElementById("slider_value_y");
sliderY.step="0.05"
sliderOutputY.innerHTML = sliderY.value; // Display the default slider value
var sliderDelay = document.getElementById("mySliderDelay");
var sliderOutputDelay = document.getElementById("slider_value_delay");
sliderDelay.step="5"
sliderOutputDelay.innerHTML = sliderDelay.value;

sliderX.oninput = function() {
	sliderOutputX.innerHTML = this.value;
	setAngles(sliderX.value,sliderY.value);
}

sliderY.oninput = function() {
	sliderOutputY.innerHTML = this.value;
	setAngles(sliderX.value,sliderY.value);
}

sliderDelay.oninput = function() {
	sliderOutputDelay.innerHTML = this.value;
	setDelayFunction(this.value);
}