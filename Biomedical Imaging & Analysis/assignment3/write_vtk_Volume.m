function write_vtk_Volume(array, Spacing, filename)

%  write_vtk_Volume: Save a 3-D scalar array in VTK format.
%  write_vtk_Volume(array, Voxel_Spacing, filename) saves a 3-D array of any size with known Voxel_Spacing (1x3 vector) to filename in VTK format.
%
% Written by Prahlad G Menon, PhD

    [nx, ny, nz] = size(array);
    fid = fopen(filename, 'wt');
    fprintf(fid, '# vtk DataFile Version 2.0\n');
    fprintf(fid, 'Comment goes here\n');
    fprintf(fid, 'ASCII\n');
    fprintf(fid, '\n');
    fprintf(fid, 'DATASET STRUCTURED_POINTS\n');
    fprintf(fid, 'DIMENSIONS    %d   %d   %d\n', nx, ny, nz);
    fprintf(fid, '\n');
    fprintf(fid, 'ORIGIN    0.000   0.000   0.000\n');
    fprintf(fid, 'SPACING   %d   %d  %d \n', Spacing(1),Spacing(2),Spacing(3));
    fprintf(fid, '\n');
    fprintf(fid, 'POINT_DATA   %d\n', nx*ny*nz);
    fprintf(fid, 'SCALARS scalars double\n');
    fprintf(fid, 'LOOKUP_TABLE default\n');
    fprintf(fid, '\n');
    for a=1:nz
        for b=1:ny
            for c=1:nx
                fprintf(fid, '%d ', array(c,b,a));
            end
            fprintf(fid, '\n');
        end
    end
    fclose(fid);
return