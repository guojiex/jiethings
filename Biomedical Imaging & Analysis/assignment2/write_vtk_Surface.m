function write_vtk_Surface(fn, pnt, dhk, vect1, vect1Magnitude)

% WRITE_VTK writes a triangulation to a Legacy VTK (Visualisation ToolKit) format file.
% Supported are triangles, tetrahedrons and hexahedrons.
%
% Use as
%   write_vtk_Surface(filename, pnt, dhk)
%
% Written by Prahlad G Menon, PhD

fid = fopen(fn, 'wt');
if fid~=-1
  
  npnt = size(pnt,1);
  ndhk = size(dhk,1);
  
  % write the header
  fprintf(fid, '# vtk DataFile Version 2.0\n');
  fprintf(fid, 'vtk output\n');
  fprintf(fid, 'ASCII\n');
  fprintf(fid, 'DATASET POLYDATA\n');
  fprintf(fid, '\n');
  
  % write the vertex points
  fprintf(fid, 'POINTS %d float\n', npnt);
  fprintf(fid, '%f\t%f\t%f\n', pnt');
  fprintf(fid, '\n');
  
  if size(dhk,2)==3
    % write the triangles
    fprintf(fid, 'POLYGONS %d %d\n', ndhk, (3+1)*ndhk);
    fprintf(fid, '3\t%d\t%d\t%d\n', (dhk-1)');
    fprintf(fid, '\n');
  elseif size(dhk,2)==4
    % write the tetrahedrons
    fprintf(fid, 'POLYGONS %d %d\n', ndhk, (4+1)*ndhk);
    fprintf(fid, '4\t%d\t%d\t%d\t%d\n', (dhk-1)');
    fprintf(fid, '\n');
  elseif size(dhk,2)==8
    % write the hexahedrons
    fprintf(fid, 'POLYGONS %d %d\n', ndhk, (8+1)*ndhk);
    fprintf(fid, '8\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n', (dhk-1)');
    fprintf(fid, '\n');
  end
  
  fprintf(fid, 'POINT_DATA %d \n', npnt); 
   fprintf(fid, 'VECTORS Surface_Normals float\n');
     fprintf(fid, '%f\t%f\t%f\n', vect1);
     fprintf(fid, '\n');
    
   fprintf(fid,'SCALARS Vector_Magnitude float \n');
   fprintf(fid,'LOOKUP_TABLE default \n');
    fprintf(fid, '%f\n', vect1Magnitude);
    fprintf(fid, '\n');

  fclose(fid);
  
else
  error('unable to open file');
end