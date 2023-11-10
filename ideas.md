# Ideas
## Bootloader
Maybe for stage0, I can use `#define`'s in stage0 to compile 4 versions, one for each disk partition possibilities, and then just use the correct one on `install` - could be cool?

And then, why the hell do we need a separate partition for boot? Same one as for root should work, no?