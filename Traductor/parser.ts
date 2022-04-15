interface Line {
  label?: string,
  mnemonic?: string,
  operands: string[],
  comment?: string,
}

function parse(line: string): Line {
  const [instr, comment] = line.split(';');

  const splitted = instr.trimStart().split(/(?<![^'].')[\s,]+/);

  return {
    label: splitted[0].endsWith(':') ? splitted.shift()?.slice(0, -1) : undefined,
    mnemonic: splitted[0] || undefined,
    operands: splitted.slice(1).filter(s => s.length),
    comment,
  };
}

const text = Deno.readTextFileSync(Deno.args[0]);
const line = text.split(/\r\n|\r|\n/)[0];
const parsed = parse(line);

console.log(parsed.label || '');
console.log(parsed.mnemonic || '');
console.log(parsed.operands[0] || '');
console.log(parsed.operands[1] || '');
console.log(parsed.comment || '');