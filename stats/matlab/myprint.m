function myprint(FileName,MatrixName,DirName,h)
filename = [DirName, '/', MatrixName, '_', FileName,'.jpg'];
%print('-dpng', '-r600', [DirName, '/', MatrixName, '_', FileName])
saveas(h,filename);
export_fig(filename,'-painters','-m2.5','-transparent');
end
