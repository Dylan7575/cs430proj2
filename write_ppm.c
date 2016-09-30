void write_p3(Pixel* pixel,int w,int h,char* filename){
    FILE *fh = fopen(filename,"w");
    fprintf(fh,"P3 %d %d 255 ",w,h);
    for(int i = 0;i<w*h;i++){
        fprintf(fh,"%d ",pixel[i].r);
        fprintf(fh,"%d ",pixel[i].g);
        fprintf(fh,"%d ",pixel[i].b);
    }
    fclose(fh);
//get height and width from command line
}