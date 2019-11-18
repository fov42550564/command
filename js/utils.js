var child_process = require('child_process');

function copy(str) {
	var command;
	switch(process.platform) {
		case "darwin":
			command = 'echo "'+str+'"|pbcopy';
			break;
		case "win32":
			command = 'mshta vbscript:clipboarddata.setdata("text","'+str+'")(close)';
			break;
		default:
			throw new Error("Unknown platform: '" + process.platform);
	}
	child_process.exec(
		command,
		function(err, stdout, stderr) {
			if (err) {
				throw new Error("copy error");
			}
		}
	);
}
function paste(callback) {
	var command;
	switch(process.platform) {
		case "darwin":
			child_process.exec(
				'pbpaste',
				function(err, stdout, stderr) {
					if (err) {
						throw new Error("copy error");
					}
					callback(stdout);
				}
			);
			break;
		case "win32":
			var paste_vbs='Set objHTML = CreateObject("htmlfile")\n'
			+'text = objHTML.ParentWindow.ClipboardData.GetData("Text")\n'
			+'Wscript.Echo text';
			fs.writeFileSync('paste.vbs', paste_vbs);
			child_process.exec('cscript /nologo paste.vbs',
				function(err, stdout, stderr) {
					if (err) {
						console.log(err);
						throw new Error("copy error");
					}
					callback(stdout);
					fs.unlinkSync('paste.vbs');
				}
			);
			break;
		default:
			throw new Error("Unknown platform: '" + process.platform);
	}
}

module.exports = {
    copy,
    paste,
}
