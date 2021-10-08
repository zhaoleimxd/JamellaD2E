import * as fs from 'fs';

function splitSrcTxt() {
    let dir: string = `E:\\Work\\cpp\\JamellaD2E\\docs`;
    let srcFilePath: string = `${dir}\\pdf.txt`;
    
    let src: string = fs.readFileSync(srcFilePath, "UTF-8");
    
    let pattern: RegExp = /\n\d+\n(.+)\n/g;
    let matches: IterableIterator<RegExpMatchArray> = src.matchAll(pattern);
    let match: IteratorResult<RegExpMatchArray> = matches.next();

    let pageCount: number = 0;
    let pages: Array<{
        fileName: string;
        content: string;
    }> = [];

    while (match.done == false) {
        console.log(`${match.value[1]}`);

        pages[pageCount] = {
            fileName: match.value[1],
            content: ""
        };

        pageCount++;
        match = matches.next();
    }

    let pattern2: RegExp = /\n\d+\n.+\n/g;
    let pageContents: Array<string> = src.split(pattern2);

    pageContents.shift();

    for (let i = 0; i < pageContents.length; i++) {
        pages[i].content = pageContents[i];
    }

    let result: {
        [fileName: string]: string;
    } = {};

    for (let i = 0; i < pages.length; i++) {
        if (result[pages[i].fileName] == undefined) {
            result[pages[i].fileName] = pages[i].content;
        }
        else {
            result[pages[i].fileName] += `\r\n` + pages[i].content;
        }
    }

    for (var fileName in result) {
        fs.writeFileSync(`${dir}\\JamellaD2E\\${fileName}`, result[fileName]);
    }
}

function copyMissingFiles() {
    let s: string = fs.readFileSync("test.txt", "UTF-8");
    let a: Array<string> = s.split("\r\n");
    let dir: string = `E:\\Work\\cpp\\JamellaD2E\\vs2019\\JamellaD2E`;
    for (let i = 0; i < a.length; i++) {
        let filePath: string = `${dir}\\${a[i]}`;
        if (fs.existsSync(filePath)) {

        }
        else {
            fs.copyFileSync("000.bmp", filePath)
        }
    }
}

(function main(){
    copyMissingFiles();
})()
