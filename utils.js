module.exports = (function() {
	function utils(){
	}

	function getFunctionName(func) {
		return (''+func).replace(/(.*?\([\s\S]*?\)).*\n[\s\S]*/, '$1');
	}
	utils.printCallStack = function() {  
		var i = 0;  
		var fun = arguments.callee;  
		do {  
			fun = fun.arguments.callee.caller;  
			console.log(i + " : " + getFunctionName(fun));  
			i++;
		} while (fun);  
	}  
	return utils;
}());
